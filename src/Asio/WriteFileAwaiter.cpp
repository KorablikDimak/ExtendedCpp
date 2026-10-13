#if defined(UNIX_IO) && defined(__APPLE__)
    #include <thread>
#endif

#include <ExtendedCpp/Asio/WriteFileAwaiter.h>

ExtendedCpp::Asio::WriteFileAwaiter::WriteFileAwaiter(
    const NativeHandle& nativeHandle,
    const OffsetType offset,
    const std::span<const std::byte> buffer) noexcept :
        FileIoAwaiter(nativeHandle, offset),
        _buffer(buffer) {}

void ExtendedCpp::Asio::WriteFileAwaiter::Start() noexcept
{
    if (_buffer.empty())
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

    dispatch_data_t payload = dispatch_data_create(_buffer.data(), _buffer.size(), _queue, nullptr);

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

            const auto written = _buffer.size() - remainingSize;

            _result = static_cast<std::streamsize>(written);

            if (error != 0)
                _exception = std::make_exception_ptr(std::system_error(error, std::generic_category()));

            dispatch_io_close(channel, 0);
            dispatch_release(channel);

            this->Resume();
        });

    dispatch_release(payload);
#elif defined(UNIX_IO)
    _control->aio_fildes = *_nativeHandle;
    _control->aio_offset = _offset;
    _control->aio_buf = const_cast<std::byte*>(_buffer.data());
    _control->aio_nbytes = _buffer.size();

#if !defined(__APPLE__)
    _control->aio_sigevent.sigev_notify = SIGEV_THREAD;
    _control->aio_sigevent.sigev_notify_function = &FileIoAwaiter::CompletionCallback;
    _control->aio_sigevent.sigev_value.sival_ptr = this; // NOLINT(*-pro-type-union-access)
#endif

    const int result = aio_write(_control.get());

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

    BOOL result = WriteFile(*_nativeHandle, _buffer.data(), static_cast<DWORD>(_buffer.size()), nullptr, &_overlapped);

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