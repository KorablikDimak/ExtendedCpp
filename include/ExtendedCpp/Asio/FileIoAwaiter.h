#ifndef Asio_FileIoAwaiter_H
#define Asio_FileIoAwaiter_H

#include <cstdint>
#include <span>

#if __APPLE__
    #define UNIX_IO 1
#elif __linux__
    #define UNIX_IO 1
#elif _WIN32
    #define WINDOWS_IO 1
#endif

#if UNIX_IO
    #include <aio.h>
#elif WINDOWS_IO
    #include <windows.h>
#endif

#include <ExtendedCpp/Asio/IoAwaiter.h>

namespace ExtendedCpp::Asio
{
#if UNIX_IO
    using NativeHandle = int;
#elif WINDOWS_IO
    using NativeHandle = HANDLE;
#endif

    class FileIoAwaiter final : public IoAwaiter
    {
    public:
        FileIoAwaiter(NativeHandle nativeHandle, std::uint64_t offset, std::span<std::byte> buffer, OperationType operationType) noexcept;

    protected:
        void Start() noexcept override;

    private:
        void Complete() noexcept;

        NativeHandle _nativeHandle;

#if UNIX_IO
        static void CompletionCallback(sigval value) noexcept;
        off_t _offset;
        aiocb _control{};
#elif WINDOWS_IO
        static DWORD WINAPI CompletionThread(void* parameter) noexcept;
        std::uint64_t _offset;
        OVERLAPPED _overlapped{};
#endif
    };
}

#endif