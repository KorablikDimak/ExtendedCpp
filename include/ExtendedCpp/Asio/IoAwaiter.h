#ifndef Asio_IoAwaiter_H
#define Asio_IoAwaiter_H

#include <coroutine>
#include <ios>

namespace ExtendedCpp::Asio
{
    class IoAwaiter
    {
    public:
        virtual ~IoAwaiter() noexcept = default;

        static bool await_ready() noexcept;
        void await_suspend(std::coroutine_handle<> continuation) noexcept;
        std::streamsize await_resume() const;

    protected:
        virtual void Start() noexcept = 0;
        void Resume() const noexcept;

        std::streamsize _result{};
        std::exception_ptr _exception;

    private:
        std::coroutine_handle<> _continuation{};
    };
}

#endif