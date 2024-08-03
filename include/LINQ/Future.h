#ifndef LINQ_Future_H
#define LINQ_Future_H

#include <coroutine>
#include <exception>

namespace LINQ
{
    template<typename TSource>
    struct Future final
    {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type final
        {
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() noexcept {}

            void unhandled_exception()
            {
                std::rethrow_exception(std::move(std::current_exception()));
            }

            std::suspend_always yield_value(TSource value) noexcept
            {
                _value = std::move(value);
                return {};
            }

            Future get_return_object()
            {
                return Future(handle_type::from_promise(*this));
            }

            TSource Value() const noexcept
            {
                return _value;
            }

        private:
            TSource _value;
        };

        explicit Future(handle_type handle) noexcept : _handle(handle) {}

        ~Future()
        {
            if (_handle) _handle.destroy();
        }

        explicit operator bool() const noexcept
        {
            return !_handle.done();
        }

        TSource Value() const noexcept
        {
            if (!_handle.done()) return _handle.promise().Value();
            else return {};
        }

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