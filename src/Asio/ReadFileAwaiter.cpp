#if defined(UNIX_IO) && defined(__APPLE__)
    #include <thread>
#endif

#include <ExtendedCpp/Asio/ReadFileAwaiter.h>

ExtendedCpp::Asio::ReadFileAwaiter::ReadFileAwaiter(
    const NativeHandle& nativeHandle,
    const OffsetType offset,
    const std::span<std::byte> buffer) noexcept :
        FileIoAwaiter(nativeHandle, offset),
        _buffer(buffer) {}

void ExtendedCpp::Asio::ReadFileAwaiter::Start() noexcept
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

    dispatch_io_read(channel, _offset, _buffer.size(), _queue,
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
                    const auto available = _buffer.size() - copied;
                    const auto count = std::min(length, available);

                    std::memcpy(_buffer.data() + copied, bytes, count);
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
#elif defined(UNIX_IO)
    _control->aio_fildes = *_nativeHandle;
    _control->aio_offset = _offset;
    _control->aio_buf = _buffer.data();
    _control->aio_nbytes = _buffer.size();

#if !defined(__APPLE__)
    _control->aio_sigevent.sigev_notify = SIGEV_THREAD;
    _control->aio_sigevent.sigev_notify_function = &FileIoAwaiter::CompletionCallback;
    _control->aio_sigevent.sigev_value.sival_ptr = this; // NOLINT(*-pro-type-union-access)
#endif

    const int result = aio_read(_control.get());

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

    BOOL result = ReadFile(*_nativeHandle, _buffer.data(), static_cast<DWORD>(_buffer.size()), nullptr, &_overlapped);

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