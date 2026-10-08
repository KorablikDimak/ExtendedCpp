#include <ExtendedCpp/Asio/IoAwaiter.h>

bool ExtendedCpp::Asio::IoAwaiter::await_ready() noexcept
{
    return false;
}

void ExtendedCpp::Asio::IoAwaiter::await_suspend(const std::coroutine_handle<> continuation) noexcept
{
    _continuation = continuation;
    Start();
}

std::streamsize ExtendedCpp::Asio::IoAwaiter::await_resume() const
{
    if (_exception)
        std::rethrow_exception(_exception);
    return _result;
}

void ExtendedCpp::Asio::IoAwaiter::Resume() const noexcept
{
    _continuation.resume();
}