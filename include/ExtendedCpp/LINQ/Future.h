#ifndef LINQ_Future_H
#define LINQ_Future_H

#include <coroutine>
#include <exception>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TSource
    /// @tparam NothrowCoroutine
    template<typename TSource, typename NothrowCoroutine = std::false_type>
    struct Future final
    {
        /// @brief
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
            std::suspend_always yield_value(TSource value) noexcept
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
            _handle(handle),
            _refCount(new std::size_t(1)) {}

        /// @brief
        /// @param other
        Future(const Future& other) noexcept
        {
            _handle = other._handle;
            _refCount = other._refCount;
            ++*_refCount;
        }

        /// @brief
        /// @param other
        Future(Future&& other) noexcept
        {
            _handle = other._handle;
            _refCount = other._refCount;
            other._handle = nullptr;
            other._refCount = nullptr;
        }

        /// @brief
        /// @param other
        /// @return
        Future& operator=(const Future& other) noexcept = delete;

        /// @brief
        /// @param other
        /// @return
        Future& operator=(Future&& other) noexcept = delete;

        /// @brief 
        ~Future()
        {
            if (!_refCount || !_handle)
                return;

		    --*_refCount;
            if (*_refCount == 0)
            {
                delete _refCount;
                _handle.destroy();
            }
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

            if constexpr (std::is_default_constructible_v<TSource>)
                return {};
            else
                std::terminate();
        }

        /// @brief 
        /// @return 
        TSource Next()
        noexcept(std::same_as<NothrowCoroutine, std::true_type>)
        {
            if (!_handle.done())
            {
                TSource value = _handle.promise().Value();
                _handle.resume();
                return value;
            }

            if constexpr (std::is_default_constructible_v<TSource>)
                return {};
            else
                std::terminate();
        }

    private:
        handle_type _handle;
        std::size_t* _refCount;
    };
}

#endif