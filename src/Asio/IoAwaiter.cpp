#include <ExtendedCpp/Asio/IoAwaiter.h>

ExtendedCpp::Asio::IoAwaiter::IoAwaiter(const std::span<std::byte> buffer, const OperationType operationType) noexcept :
    _buffer(buffer),
    _operationType(operationType) {}

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