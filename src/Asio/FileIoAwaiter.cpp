#include <ExtendedCpp/Asio/FileIoAwaiter.h>

ExtendedCpp::Asio::FileIoAwaiter::FileIoAwaiter(
    const NativeHandle& nativeHandle,
    const std::uint64_t offset,
    const std::span<std::byte> buffer) noexcept :
        _nativeHandle(nativeHandle),
        _buffer(buffer),
        _offset(offset),
        _operationType(OperationType::Read) {}

ExtendedCpp::Asio::FileIoAwaiter::FileIoAwaiter(
    const NativeHandle& nativeHandle,
    const std::uint64_t offset,
    const std::span<const std::byte> buffer) noexcept :
        _nativeHandle(nativeHandle),
        _buffer(buffer),
        _offset(offset),
        _operationType(OperationType::Write) {}

void ExtendedCpp::Asio::FileIoAwaiter::Start() noexcept
{
#if UNIX_IO
    _control->aio_fildes = *_nativeHandle;
    _control->aio_offset = _offset;

    if (_operationType == OperationType::Read)
    {
        const auto buffer = std::get<std::span<std::byte>>(_buffer);
        _control->aio_buf = buffer.data();
        _control->aio_nbytes = buffer.size();
    }
    else
    {
        const auto buffer = std::get<std::span<const std::byte>>(_buffer);
        _control->aio_buf = const_cast<std::byte*>(buffer.data());
        _control->aio_nbytes = buffer.size();
    }

    _control->aio_sigevent.sigev_notify = SIGEV_THREAD;
    _control->aio_sigevent.sigev_notify_function = &FileIoAwaiter::CompletionCallback;
    _control->aio_sigevent.sigev_value.sival_ptr = this;

    int result{};

    switch (_operationType)
    {
        case OperationType::Read:
            result = aio_read(_control.get());
            break;
        case OperationType::Write:
            result = aio_write(_control.get());
            break;
    }

    if (result != 0)
    {
        _exception = std::make_exception_ptr(std::system_error(errno, std::generic_category()));
        Resume();
    }
#elif WINDOWS_IO
    _overlapped = {};
    _overlapped.Offset = _offset & 0xFFFFFFFF;
    _overlapped.OffsetHigh = _offset >> 32;

    BOOL result{};
    switch (_operationType)
    {
        case OperationType::Read:
            result = ReadFile(*_nativeHandle,
                std::get<std::span<std::byte>>(_buffer).data(),
                std::get<std::span<std::byte>>(_buffer).size(),
                nullptr, &_overlapped);
            break;

        case OperationType::Write:
            result = WriteFile(*_nativeHandle,
                std::get<std::span<const std::byte>>(_buffer).data(),
                std::get<std::span<const std::byte>>(_buffer).size(),
                nullptr, &_overlapped);
            break;
    }

    if (result)
    {
        Complete();
        return;
    }

    const DWORD error = GetLastError();

    if (error != ERROR_IO_PENDING)
    {
        _exception = std::make_exception_ptr(std::system_error(error, std::system_category()));
        Resume();
        return;
    }

    const HANDLE thread = CreateThread(nullptr, 0, &FileIoAwaiter::CompletionThread, this, 0, nullptr);

    if (thread == nullptr)
    {
        _exception = std::make_exception_ptr(std::system_error(GetLastError(), std::system_category()));
        Resume();
        return;
    }

    CloseHandle(thread);
#endif
}

void ExtendedCpp::Asio::FileIoAwaiter::Complete() noexcept
{
#if UNIX_IO
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
#elif WINDOWS_IO
    DWORD transferred{};

    const BOOL result = GetOverlappedResult(*_nativeHandle, &_overlapped, &transferred, TRUE);

    if (!result)
    {
        const DWORD error = GetLastError();
        _exception = std::make_exception_ptr(std::system_error(error, std::system_category()));
        Resume();
        return;
    }

    _result = transferred;
    Resume();
#endif
}

#if UNIX_IO
void ExtendedCpp::Asio::FileIoAwaiter::CompletionCallback(const sigval value) noexcept
{
    auto* awaiter = static_cast<FileIoAwaiter*>(value.sival_ptr);
    awaiter->Complete();
}
#elif WINDOWS_IO
DWORD ExtendedCpp::Asio::FileIoAwaiter::CompletionThread(void* parameter) noexcept
{
    auto* awaiter = static_cast<FileIoAwaiter*>(parameter);
    awaiter->Complete();
    return 0;
}
#endif