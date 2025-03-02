#ifndef Common_Task_H
#define Common_Task_H

#include <coroutine>
#include <exception>
#include <future>

/// @brief 
namespace ExtendedCpp
{
    /// @brief 
    /// @tparam TResult 
    template<typename TResult>
    struct Future final
    {
        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        template<typename TOperation, typename... Args>
        requires std::same_as<std::invoke_result_t<TOperation, Args...>, TResult>
        explicit Future(TOperation&& operation, Args&&... args) 
        {
            _asyncTask = std::async(std::launch::async, 
            [ this, operation = std::forward<TOperation>(operation), ...args = std::forward<Args>(args) ]()
            {
                auto temp = std::move(operation(args...));
            
                std::lock_guard lock(_mutex);
                _result = std::move(temp);
                if (_handle.has_value())
                    _handle.value().resume();
            });
        }

        /// @brief 
        /// @return 
        bool await_ready() const noexcept 
        { 
            return true;
        }

        /// @brief 
        /// @param handle 
        void await_suspend(std::coroutine_handle<> handle) noexcept
        {
            std::lock_guard lock(_mutex);
            if (_result.has_value())
                handle.resume();
            else
                _handle = std::move(handle);
        }
        
        /// @brief 
        /// @return 
        TResult await_resume() const noexcept 
        {
            return std::move(_result.value());
        }

        TResult Result() const noexcept
        {
            while (true) 
            {
                {
                    std::lock_guard lock(_mutex);
                    if (_result.has_value())
                        break;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            return std::move(_result.value());
        }

    private:
        std::future<void> _asyncTask;
        mutable std::mutex _mutex;
        std::optional<std::coroutine_handle<>> _handle = std::nullopt;
        std::optional<TResult> _result  = std::nullopt;
    };

    /// @brief 
    template<>
    struct Future<void> final
    {
        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        template<typename TOperation, typename... Args>
        requires std::same_as<std::invoke_result_t<TOperation, Args...>, void>
        explicit Future(TOperation&& operation, Args&&... args) 
        {
            _asyncTask = std::async(std::launch::async, 
            [ this, operation = std::forward<TOperation>(operation), ...args = std::forward<Args>(args) ]()
            {
                auto temp = std::move(operation(std::forward<Args>(args)...));
            
                std::lock_guard lock(_mutex);
                _isFinished = true;
                if (_handle.has_value())
                    _handle.value().resume();
            });
        }

        /// @brief 
        /// @return 
        bool await_ready() const noexcept 
        { 
            return true;
        }

        /// @brief 
        /// @param handle 
        void await_suspend(std::coroutine_handle<> handle) noexcept
        {
            std::lock_guard lock(_mutex);
            if (_isFinished)
                handle.resume();
            else
                _handle = std::move(handle);
        }
        
        /// @brief 
        void await_resume() const noexcept {}

        void Wait() const noexcept
        {
            while (true) 
            {
                {
                    std::lock_guard lock(_mutex);
                    if (_isFinished)
                        break;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }

    private:
        std::future<void> _asyncTask;
        mutable std::mutex _mutex;
        std::optional<std::coroutine_handle<>> _handle = std::nullopt;
        bool _isFinished = false;
    };

    /// @brief 
    struct Task final
    {
        /// @brief 
        struct promise_type final
        {
            /// @brief 
            /// @return 
            std::suspend_never initial_suspend() noexcept { return {}; }

            /// @brief 
            /// @return 
            std::suspend_never final_suspend() noexcept { return {}; }
                
            /// @brief 
            void return_void() noexcept {}

            /// @brief 
            void unhandled_exception()
            {
                std::rethrow_exception(std::current_exception());
            }

            /// @brief 
            /// @return 
            Task get_return_object() noexcept { return {}; }
        };

        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        /// @return 
        template<typename TOperation, typename... Args>
        static Future<std::invoke_result_t<TOperation, Args...>> Run(TOperation&& operation, Args&&... args)
        {
            return Future<std::invoke_result_t<TOperation, Args...>>
                (std::forward<TOperation>(operation), std::forward<Args>(args)...);
        }
    };
}

#endif