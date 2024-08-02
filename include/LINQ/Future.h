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

            TSource Value() noexcept
            {
                return std::move(_value);
            }

        private:
            TSource _value;
        };

        explicit Future(handle_type handle) noexcept : _handle(handle) {}

        ~Future()
        {
            _handle.destroy();
        }

        explicit operator bool() noexcept
        {
            return !_handle.done();
        }

        TSource Next() noexcept
        {
            TSource value = std::move(_handle.promise().Value());
            _handle.resume();
            return value;
        }

    private:
        handle_type _handle;
    };
}

#endif