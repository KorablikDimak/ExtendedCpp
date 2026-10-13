#ifndef ASIO_FILE_IO_AWAITER_H
#define ASIO_FILE_IO_AWAITER_H

#if defined(DARWIN_IO)
    #include <dispatch/dispatch.h>
#elif defined(UNIX_IO)
    #include <memory>
    #include <aio.h>
#elif defined(WINDOWS_IO)
    #include <windows.h>
#endif

#include <ExtendedCpp/Asio/IoAwaiter.h>
#include <ExtendedCpp/Asio/NativeHandle.h>

namespace ExtendedCpp::Asio
{
#if defined(UNIX_IO) || defined(DARWIN_IO)
    using OffsetType = off_t;
#elif defined(WINDOWS_IO)
    using OffsetType = std::int64_t;
#endif

    class FileIoAwaiter : public IoAwaiter
    {
    public:
        FileIoAwaiter(const NativeHandle& nativeHandle, OffsetType offset) noexcept;

        FileIoAwaiter(const FileIoAwaiter& other) noexcept = delete;
        FileIoAwaiter(FileIoAwaiter&& other) noexcept = delete;

        FileIoAwaiter& operator=(const FileIoAwaiter& other) noexcept = delete;
        FileIoAwaiter& operator=(FileIoAwaiter&& other) noexcept = delete;

        ~FileIoAwaiter() noexcept override = default;

    protected:
    #if defined(DARWIN_IO)
        dispatch_queue_t _queue = dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0);
    #elif defined(UNIX_IO)
        void Complete() noexcept;
        static void CompletionCallback(sigval value) noexcept;
        std::unique_ptr<aiocb> _control = std::make_unique<aiocb>();
    #elif defined(WINDOWS_IO)
        void Complete() noexcept;
        static DWORD WINAPI CompletionThread(void* parameter) noexcept;
        OVERLAPPED _overlapped{};
    #endif

        const NativeHandle& _nativeHandle;
        const OffsetType _offset;
    };
}

#endif