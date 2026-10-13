#if defined(UNIX_IO) && defined(__APPLE__)
    #include <thread>
#endif

#include <ExtendedCpp/Asio/FileIoAwaiter.h>

ExtendedCpp::Asio::FileIoAwaiter::FileIoAwaiter(
    const NativeHandle& nativeHandle,
    const OffsetType offset,
    const std::span<std::byte> buffer) noexcept :
        _nativeHandle(nativeHandle),
        _buffer(buffer),
        _offset(offset),
        _operationType(OperationType::Read) {}

ExtendedCpp::Asio::FileIoAwaiter::FileIoAwaiter(
    const NativeHandle& nativeHandle,
    const OffsetType offset,
    const std::span<const std::byte> buffer) noexcept :
        _nativeHandle(nativeHandle),
        _buffer(buffer),
        _offset(offset),
        _operationType(OperationType::Write) {}

void ExtendedCpp::Asio::FileIoAwaiter::Start() noexcept
{
    if (IsEmptyBuffer())
    {
        _result = 0;
        Resume();
        return;
    }

#if defined(DARWIN_IO)
    dispatch_io_t channel = dispatch_io_create(DISPATCH_IO_RANDOM, *_nativeHandle, _queue, {});

    if (channel == nullptr)
    {
        _exception = std::make_exception_ptr(std::system_error(errno, std::generic_category()));
        Resume();
        return;
    }

    if (_operationType == OperationType::Read)
    {
        const auto& span = std::get<std::span<std::byte>>(_buffer);

        dispatch_io_read(channel, _offset, span.size(), _queue,
            ^(const bool isDone, const dispatch_data_t data, const int error)
            {
                if (data)
                {
                    const void* bytes{};
                    std::size_t length{};

                    const dispatch_data_t mapped = dispatch_data_create_map(data, &bytes, &length);

                    if (mapped == nullptr)
                        _exception = std::make_exception_ptr(std::bad_alloc());
                    else if (length != 0)
                    {
                        const auto copied = static_cast<std::size_t>(_result);
                        const auto available = span.size() - copied;
                        const auto count = std::min(length, available);

                        std::memcpy(span.data() + copied, bytes, count);
                        _result += static_cast<std::streamsize>(count);
                    }

                    if (mapped != nullptr)
                        dispatch_release(mapped);
                }

                if (error != 0)
                    _exception = std::make_exception_ptr(std::system_error(error, std::generic_category()));

                if (!isDone)
                    return;

                dispatch_io_close(channel, 0);
                dispatch_release(channel);

                Resume();
            });
    }
    else
    {
        const auto span = std::get<std::span<const std::byte>>(_buffer);

        dispatch_data_t payload = dispatch_data_create(span.data(), span.size(), _queue, nullptr);

        if (payload == nullptr)
        {
            dispatch_io_close(channel, 0);
            dispatch_release(channel);
            _exception = std::make_exception_ptr(std::bad_alloc());
            Resume();
            return;
        }

        dispatch_io_write(channel, _offset, payload, _queue,
            ^(const bool done, const dispatch_data_t remaining, const int error)
            {
                if (!done)
                    return;

                const std::size_t remainingSize =
                    remaining == nullptr ? 0 : dispatch_data_get_size(remaining);

                const auto written = span.size() - remainingSize;

                _result = static_cast<std::streamsize>(written);

                if (error != 0)
                    _exception = std::make_exception_ptr(std::system_error(error, std::generic_category()));

                dispatch_io_close(channel, 0);
                dispatch_release(channel);

                this->Resume();
            });

        dispatch_release(payload);
    }
#elif defined(UNIX_IO)
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

#if !defined(__APPLE__)
    _control->aio_sigevent.sigev_notify = SIGEV_THREAD;
    _control->aio_sigevent.sigev_notify_function = &FileIoAwaiter::CompletionCallback;
    _control->aio_sigevent.sigev_value.sival_ptr = this; // NOLINT(*-pro-type-union-access)
#endif

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
        return;
    }

#if defined(__APPLE__)
    std::thread([this]() noexcept
    {
        while (true)
        {
            const int error = aio_error(_control.get());

            if (error == EINPROGRESS)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            else
                break;
        }
        CompletionCallback(sigval{ .sival_ptr = this });
    }).detach();
#endif

#elif defined(WINDOWS_IO)
    _overlapped = {};
    _overlapped.Offset = _offset & 0xFFFFFFFF;
    _overlapped.OffsetHigh = _offset >> 32;

    BOOL result{};
    switch (_operationType)
    {
        case OperationType::Read:
            result = ReadFile(*_nativeHandle,
                std::get<std::span<std::byte>>(_buffer).data(),
                static_cast<DWORD>(std::get<std::span<std::byte>>(_buffer).size()),
                nullptr, &_overlapped);
            break;

        case OperationType::Write:
            result = WriteFile(*_nativeHandle,
                std::get<std::span<const std::byte>>(_buffer).data(),
                static_cast<DWORD>(std::get<std::span<const std::byte>>(_buffer).size()),
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

bool ExtendedCpp::Asio::FileIoAwaiter::IsEmptyBuffer() const noexcept
{
    return
        (_operationType == OperationType::Read && std::get<std::span<std::byte>>(_buffer).empty()) ||
        (_operationType == OperationType::Write && std::get<std::span<const std::byte>>(_buffer).empty());
}

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

        if (_operationType == OperationType::Read && error == ERROR_HANDLE_EOF)
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