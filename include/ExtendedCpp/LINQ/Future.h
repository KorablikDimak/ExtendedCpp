#ifndef LINQ_Future_H
#define LINQ_Future_H

#include <coroutine>
#include <exception>
#include <iostream>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TSource
    /// @tparam NothrowCoroutine
    template<typename TSource, typename NothrowCoroutine = std::false_type>
    class Future final
    {
    private:
        /// @brief 
        struct Promise final
        {
            /// @brief 
            /// @return 
            [[nodiscard]]
            std::suspend_never initial_suspend() const noexcept { return {}; }

            /// @brief 
            /// @return 
            [[nodiscard]]
            std::suspend_always final_suspend() const noexcept { return {}; }
            
            /// @brief 
            void return_void() const noexcept {}

            /// @brief 
            void unhandled_exception() const
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
                return Future(std::coroutine_handle<Promise>::from_promise(*this));
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

    public:
        /// @brief
        using promise_type = Promise;

        /// @brief 
        /// @param handle 
        explicit Future(std::coroutine_handle<Promise> handle) noexcept :
            _refCount(new std::size_t(1)),
            _handle(handle) {}

        /// @brief
        /// @param other
        Future(const Future& other) noexcept :
            _refCount(other._refCount),
            _handle(other._handle)
        {
            ++*_refCount;
        }

        /// @brief
        /// @param other
        Future(Future&& other) noexcept :
            _refCount(other._refCount),
            _handle(other._handle)
        {
            ++*_refCount;
            other.Release();
        }

        /// @brief
        /// @param other
        /// @return
        Future& operator=(const Future& other) noexcept
        {
            if (this == &other)
                return *this;

            Release();

            _refCount = other._refCount;
            _handle = other._handle;
            ++*_refCount;

            return *this;
        }

        /// @brief
        /// @param other
        /// @return
        Future& operator=(Future&& other) noexcept
        {
            if (this == &other)
                return *this;

            Release();

            _refCount = other._refCount;
            _handle = other._handle;
            ++*_refCount;

            other.Release();

            return *this;
        }

        /// @brief 
        ~Future()
        {
            Release();
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
        void Release() noexcept
        {
            if (_refCount)
            {
                if (*_refCount == 1)
                {
                    delete _refCount;
                    _refCount = nullptr;
                    _handle.destroy();
                }
                else
                    --*_refCount;
            }
        }

        std::size_t* _refCount;
        std::coroutine_handle<Promise> _handle;
    };
}

#endif