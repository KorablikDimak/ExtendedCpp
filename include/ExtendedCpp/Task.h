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
            std::lock_guard lock(_mutex);
            return _result.has_value();
        }

        /// @brief 
        /// @param handle 
        void await_suspend(std::coroutine_handle<> handle)
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

    private:
        std::future<void> _asyncTask;
        mutable std::mutex _mutex;
        std::optional<std::coroutine_handle<>> _handle = std::nullopt;
        std::optional<TResult> _result = std::nullopt;
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
            std::lock_guard lock(_mutex);
            return _isFinished;
        }

        /// @brief 
        /// @param handle 
        void await_suspend(std::coroutine_handle<> handle)
        {
            std::lock_guard lock(_mutex);
            if (_isFinished)
                handle.resume();
            else
                _handle = std::move(handle);
        }
        
        /// @brief 
        void await_resume() const noexcept {}

    private:
        std::future<void> _asyncTask;
        mutable std::mutex _mutex;
        std::optional<std::coroutine_handle<>> _handle = std::nullopt;
        bool _isFinished = false;
    };

    /// @brief 
    template<typename TResult>
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
            /// @param result 
            void return_value(TResult&& result) noexcept 
            { 
                _result = std::move(result); 
                _isDone.store(true);
            }

            /// @brief 
            void unhandled_exception() 
            { 
                std::rethrow_exception(std::current_exception()); 
            }

            /// @brief 
            /// @return 
            Task get_return_object()    
            { 
                return Task(std::coroutine_handle<promise_type>::from_promise(*this)); 
            }

            /// @brief 
            /// @return 
            bool IsDone() const noexcept
            {
                return _isDone.load(); 
            }

            /// @brief 
            /// @return 
            TResult Result() noexcept 
            { 
                return std::move(_result); 
            }

        private:
            TResult _result;
            std::atomic<bool> _isDone;
        };

        /// @brief 
        /// @param handle 
        explicit Task(std::coroutine_handle<promise_type> handle) noexcept : _handle(handle) {};

        /// @brief 
        void Wait() const noexcept
        {
            while (true)
            {
                if (_handle.promise().IsDone()) 
                    return;
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }

        /// @brief 
        /// @return 
        TResult Result() const noexcept
        {
            while (true)
            {
                if (_handle.promise().IsDone())
                    return _handle.promise().Result();
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }

        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        /// @return 
        template<typename TOperation, typename... Args>
        requires std::invocable<TOperation, Args...>
        static Future<std::invoke_result_t<TOperation, Args...>> Run(TOperation&& operation, Args&&... args)
        {
            return Future<std::invoke_result_t<TOperation, Args...>>
                (std::forward<TOperation>(operation), std::forward<Args>(args)...);
        }

    private:
        std::coroutine_handle<promise_type> _handle;
    };

    /// @brief 
    template<>
    struct Task<void> final
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
            void return_void() noexcept 
            { 
                _isDone.store(true); 
            }

            /// @brief 
            void unhandled_exception() 
            { 
                std::rethrow_exception(std::current_exception()); 
            }

            /// @brief 
            /// @return 
            Task get_return_object() 
            { 
                return Task(std::coroutine_handle<promise_type>::from_promise(*this)); 
            }

            bool IsDone() const noexcept
            {
                return _isDone.load();
            }

        private:
            std::atomic<bool> _isDone;
        };

        /// @brief 
        /// @param handle 
        explicit Task(std::coroutine_handle<promise_type> handle) noexcept : _handle(handle) {};

        /// @brief 
        void Wait() const noexcept
        {
            while (true)
            {
                if (_handle.promise().IsDone()) 
                    return;
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }

        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        /// @return 
        template<typename TOperation, typename... Args>
        requires std::same_as<std::invoke_result_t<TOperation, Args...>, void>
        static Future<void> Run(TOperation&& operation, Args&&... args)
        {
            return Future<void>(std::forward<TOperation>(operation), std::forward<Args>(args)...);
        }

    private:
        std::coroutine_handle<promise_type> _handle;
    };
}

#endif