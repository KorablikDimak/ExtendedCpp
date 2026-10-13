#ifndef ASIO_IO_AWAITER_H
#define ASIO_IO_AWAITER_H

#include <coroutine>
#include <ios>

namespace ExtendedCpp::Asio
{
    class IoAwaiter
    {
    public:
        virtual ~IoAwaiter() noexcept = default;

        [[nodiscard]]
        bool await_ready() const noexcept;

        void await_suspend(std::coroutine_handle<> continuation) noexcept;

        [[nodiscard]]
        std::streamsize await_resume() const;

    protected:
        virtual void Start() noexcept = 0;
        void Resume() const noexcept;

        std::streamsize _result{};
        std::exception_ptr _exception;

    private:
        std::coroutine_handle<> _continuation;
    };
}

#endif