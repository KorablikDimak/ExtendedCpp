#ifndef LINQ_Future_H
#define LINQ_Future_H

#include <coroutine>
#include <exception>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief 
    /// @tparam TSource 
    template<typename TSource>
    struct Future final
    {
        struct promise_type;

        /// @brief 
        using value_type = TSource;

        /// @brief 
        using promise_type = promise_type;

        /// @brief 
        using handle_type = std::coroutine_handle<promise_type>;

        /// @brief 
        struct promise_type final
        {
            /// @brief 
            /// @return 
            std::suspend_never initial_suspend() noexcept { return {}; }

            /// @brief 
            /// @return 
            std::suspend_always final_suspend() noexcept { return {}; }
            
            /// @brief 
            void return_void() noexcept {}

            /// @brief 
            void unhandled_exception()
            {
                std::rethrow_exception(std::current_exception());
            }

            /// @brief 
            /// @param value 
            /// @return 
            std::suspend_always yield_value(TSource value) 
            noexcept(std::is_nothrow_move_assignable_v<TSource>)
            {
                _value = std::move(value);
                return {};
            }

            /// @brief 
            /// @return 
            Future get_return_object() noexcept
            {
                return Future(handle_type::from_promise(*this));
            }

            /// @brief 
            /// @return 
            TSource Value() const noexcept
            {
                return _value;
            }

        private:
            TSource _value;
        };

        /// @brief 
        /// @param handle 
        explicit Future(handle_type handle) noexcept :
            _handle(handle) {}

        /// @brief 
        ~Future()
        {
            if (_handle)
                _handle.destroy();
            _handle = nullptr;
        }

        /// @brief 
        explicit operator bool() const noexcept
        {
            return !_handle.done();
        }

        /// @brief 
        /// @return 
        TSource Value() const noexcept
        {
            if (!_handle.done())
                return _handle.promise().Value();
            else
                return {};
        }

        /// @brief 
        /// @return 
        TSource Next() noexcept
        {
            if (!_handle.done())
            {
                TSource value = _handle.promise().Value();
                _handle.resume();
                return value;
            }
            else return {};
        }

    private:
        handle_type _handle;
    };
}

#endif