#include <ExtendedCpp/Asio/IoAwaiter.h>

// ReSharper disable once CppMemberFunctionMayBeStatic
bool ExtendedCpp::Asio::IoAwaiter::await_ready() const noexcept // NOLINT(*-convert-member-functions-to-static)
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