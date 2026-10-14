// ReSharper disable CppMemberFunctionMayBeStatic
#ifndef LINQ_FUTURE_H
#define LINQ_FUTURE_H

#include <coroutine>
#include <exception>
#include <optional>

/// @brief
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TSource
    /// @tparam NothrowCoroutine
    template<std::copyable TSource, bool NothrowCoroutine>
    class Future final
    {
    private:
        struct Promise final
        {
            [[nodiscard]]
            std::suspend_always initial_suspend() const noexcept { return {}; }

            [[nodiscard]]
            std::suspend_always final_suspend() const noexcept { return {}; }

            void unhandled_exception() noexcept
            {
                Exception = std::current_exception();
            }

            void return_void() const noexcept {}

            template<typename TValue>
            requires std::constructible_from<TSource, TValue&&>
            std::suspend_always yield_value(TValue&& value)
            noexcept(std::is_nothrow_constructible_v<TSource, TValue&&>)
            {
                Value.emplace(std::forward<TValue>(value));
                return {};
            }

            Future get_return_object() noexcept
            {
                return Future(std::coroutine_handle<Promise>::from_promise(*this));
            }

            std::optional<TSource> Value;
            std::exception_ptr Exception;
        };

    public:
        /// @brief
        using promise_type = Promise;

        /// @brief 
        /// @param handle
        explicit Future(std::coroutine_handle<Promise> handle) noexcept :
            _handle(handle) {}

        /// @brief
        /// @param other
        Future(const Future& other) noexcept = delete;

        ///
        /// @param other
        Future(Future&& other) noexcept :
            _handle(std::exchange(other._handle, {})) {}

        /// @brief
        /// @param other
        /// @return
        Future& operator=(const Future& other) noexcept = delete;

        /// @brief
        /// @param other
        /// @return
        Future& operator=(Future&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                _handle = std::exchange(other._handle, {});
            }

            return *this;
        }

        /// @brief
        ~Future() noexcept
        {
            Release();
        }

        /// @brief
        explicit operator bool() const noexcept
        {
            return _handle && !_handle.done();
        }

        /// @brief 
        /// @return
        const TSource& Value() const noexcept
        {
            return _handle.promise().Value.value();
        }

        /// @brief 
        /// @return
        bool Next() noexcept(NothrowCoroutine)
        {
            if (!_handle || _handle.done())
            {
                if constexpr(NothrowCoroutine)
                    RethrowIfNeeded();
                return false;
            }

            _handle.promise().Value.reset();
            _handle.resume();

            if constexpr(!NothrowCoroutine)
                RethrowIfNeeded();

            return _handle.promise().Value.has_value();
        }

    private:
        void RethrowIfNeeded() const
        {
            if (_handle && _handle.promise().Exception)
                std::rethrow_exception(_handle.promise().Exception);
        }

        void Release() noexcept
        {
            if (_handle)
            {
                _handle.destroy();
                _handle = {};
            }
        }

        std::coroutine_handle<Promise> _handle;
    };
}

#endif