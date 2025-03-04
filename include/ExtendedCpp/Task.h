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
    template<typename TResult = void>
    struct Task final
    {
        /// @brief 
        struct Promise final
        {
            /// @brief 
            /// @return 
            std::suspend_never initial_suspend() noexcept { return {}; }

            /// @brief 
            /// @return 
            std::suspend_always final_suspend() noexcept { return {}; }

            /// @brief
            /// @param result 
            void return_value(TResult&& result) noexcept 
            { 
                _result = std::move(result); 
                _isDone.store(true);

                if (_continuation)
                    _continuation.resume();
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
                return Task(std::coroutine_handle<Promise>::from_promise(*this)); 
            }

            /// @brief 
            /// @param continuation 
            void SetContinuation(std::coroutine_handle<Promise> continuation)
            {
                _continuation = continuation;
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
            std::coroutine_handle<Promise> _continuation;
            TResult _result;
            std::atomic<bool> _isDone;
        };

        /// @brief 
        using promise_type = Promise;

        /// @brief 
        struct InitialAwaiter final
        {
            /// @brief 
            /// @tparam TOperation 
            /// @tparam ...Args 
            /// @param operation 
            /// @param ...args 
            template<typename TOperation, typename... Args>
            requires std::same_as<std::invoke_result_t<TOperation, Args...>, TResult>
            explicit InitialAwaiter(TOperation&& operation, Args&&... args) 
            {
                _asyncTask = std::async(std::launch::async, 
                [ this, operation = std::forward<TOperation>(operation), ...args = std::forward<Args>(args) ]()
                {
                    TResult temp = operation(args...);
                
                    std::lock_guard lock(_mutex);
                    _result = std::move(temp);
                    if (_handle)
                        _handle.resume();
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
            void await_suspend(std::coroutine_handle<Promise> handle)
            {
                std::lock_guard lock(_mutex);
                if (_result.has_value())
                    handle.resume();
                else
                    _handle = handle;
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
            std::coroutine_handle<Promise> _handle;
            std::optional<TResult> _result = std::nullopt;
        };

        /// @brief 
        struct ContinueAwaiter
        {
            /// @brief 
            /// @return 
            bool await_ready() const noexcept 
            { 
                return !_handle || _handle.promise().IsDone();
            }

            /// @brief 
            /// @param continuation 
            void await_suspend(std::coroutine_handle<Promise> continuation)
            {
                _handle.promise().SetContinuation(continuation);
            }

            /// @brief 
            /// @return 
            TResult await_resume() noexcept 
            requires (!std::same_as<TResult, void>)
            { 
                return std::move(_handle.promise().Result()); 
            }

            void await_resume() noexcept 
            requires std::same_as<TResult, void> {}

            /// @brief 
            /// @param handle 
            explicit ContinueAwaiter(std::coroutine_handle<Promise> handle) noexcept : _handle(handle) {};

        private:
            std::coroutine_handle<Promise> _handle;
        };

        /// @brief 
        /// @param handle 
        explicit Task(std::coroutine_handle<Promise> handle) noexcept : _handle(handle) {};

        /// @brief 
        /// @return 
        ContinueAwaiter operator co_await()
        {
            return ContinueAwaiter(_handle);
        }

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
        requires (!std::same_as<TResult, void>)
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
        requires std::convertible_to<std::invoke_result_t<TOperation, Args...>, TResult>
        static Task Run(TOperation&& operation, Args&&... args)
        {
            co_return co_await InitialAwaiter(std::forward<TOperation>(operation), std::forward<Args>(args)...);
        }

    private:
        std::coroutine_handle<Promise> _handle;
    };

    /// @brief 
    template<>
    struct Task<void>::Promise
    {
        /// @brief 
        /// @return 
        std::suspend_never initial_suspend() noexcept { return {}; }

        /// @brief 
        /// @return 
        std::suspend_always final_suspend() noexcept { return {}; }

        /// @brief 
        /// @param  
        void return_value(bool&&) noexcept 
        { 
            _isDone.store(true);

            if (_continuation)
                _continuation.resume();
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
            return Task(std::coroutine_handle<Promise>::from_promise(*this)); 
        }

        /// @brief 
        /// @param continuation 
        void SetContinuation(std::coroutine_handle<Promise> continuation)
        {
            _continuation = continuation;
        }

        /// @brief 
        /// @return 
        bool IsDone() const noexcept
        {
            return _isDone.load(); 
        }

    private:
        std::coroutine_handle<Promise> _continuation;
        std::atomic<bool> _isDone;
    };

    /// @brief 
    template<>
    struct Task<void>::InitialAwaiter
    {
        /// @brief 
        /// @tparam TOperation 
        /// @tparam ...Args 
        /// @param operation 
        /// @param ...args 
        template<typename TOperation, typename... Args>
        requires std::same_as<std::invoke_result_t<TOperation, Args...>, void>
        explicit InitialAwaiter(TOperation&& operation, Args&&... args) 
        {
            _asyncTask = std::async(std::launch::async, 
            [ this, operation = std::forward<TOperation>(operation), ...args = std::forward<Args>(args) ]()
            {
                operation(args...);
                
                std::lock_guard lock(_mutex);
                _isFinished = true;
                if (_handle)
                    _handle.resume();
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
        void await_suspend(std::coroutine_handle<Promise> handle)
        {
            std::lock_guard lock(_mutex);
            if (_isFinished)
                handle.resume();
            else
                _handle = handle;
        }
            
        /// @brief 
        /// @return 
        bool await_resume() const noexcept 
        { 
            return  true; 
        }

    private:
        std::future<void> _asyncTask;
        mutable std::mutex _mutex;
        std::coroutine_handle<Promise> _handle;
        bool _isFinished = false;
    };
}

#endif