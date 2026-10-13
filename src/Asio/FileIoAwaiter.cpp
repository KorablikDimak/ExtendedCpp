#include <ExtendedCpp/Asio/FileIoAwaiter.h>

ExtendedCpp::Asio::FileIoAwaiter::FileIoAwaiter(
    const NativeHandle& nativeHandle,
    const OffsetType offset) noexcept :
        _nativeHandle(nativeHandle),
        _offset(offset) {}

#if defined(UNIX_IO)
void ExtendedCpp::Asio::FileIoAwaiter::Complete() noexcept
{
    const int error = aio_error(_control.get());

    if (error != 0)
    {
        _exception = std::make_exception_ptr(std::system_error(error, std::generic_category()));
        Resume();
        return;
    }

    const std::streamsize result = aio_return(_control.get());

    if (result < 0)
    {
        _exception = std::make_exception_ptr(std::system_error(errno, std::generic_category()));
        Resume();
        return;
    }

    _result = result;
    Resume();
}

void ExtendedCpp::Asio::FileIoAwaiter::CompletionCallback(const sigval value) noexcept
{
    auto* awaiter = static_cast<FileIoAwaiter*>(value.sival_ptr); // NOLINT(*-pro-type-union-access)
    awaiter->Complete();
}
#elif defined(WINDOWS_IO)
void ExtendedCpp::Asio::FileIoAwaiter::Complete() noexcept
{
    DWORD transferred{};

    const BOOL result = GetOverlappedResult(*_nativeHandle, &_overlapped, &transferred, TRUE);

    if (!result)
    {
        const DWORD error = GetLastError();

        if (error == ERROR_HANDLE_EOF)
        {
            _result = 0;
            Resume();
            return;
        }

        _exception = std::make_exception_ptr(std::system_error(error, std::system_category()));
        Resume();
        return;
    }

    _result = transferred;
    Resume();
}

DWORD ExtendedCpp::Asio::FileIoAwaiter::CompletionThread(void* parameter) noexcept
{
    auto* awaiter = static_cast<FileIoAwaiter*>(parameter);
    awaiter->Complete();
    return 0;
}
#endif