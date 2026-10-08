#ifndef Asio_FileIoAwaiter_H
#define Asio_FileIoAwaiter_H

#include <span>
#include <variant>
#include <memory>

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
#include <ExtendedCpp/Asio/NativeHandle.h>

namespace ExtendedCpp::Asio
{
    class FileIoAwaiter final : public IoAwaiter
    {
    public:
        FileIoAwaiter(const NativeHandle& nativeHandle, std::uint64_t offset, std::span<std::byte> buffer) noexcept;
        FileIoAwaiter(const NativeHandle& nativeHandle, std::uint64_t offset, std::span<const std::byte> buffer) noexcept;

        FileIoAwaiter(const FileIoAwaiter& other) noexcept = delete;
        FileIoAwaiter(FileIoAwaiter&& other) noexcept = delete;

        FileIoAwaiter& operator=(const FileIoAwaiter& other) noexcept = delete;
        FileIoAwaiter& operator=(FileIoAwaiter&& other) noexcept = delete;

        ~FileIoAwaiter() noexcept override = default;

    protected:
        void Start() noexcept override;

    private:
        enum class OperationType
        {
            Read,
            Write
        };

        void Complete() noexcept;

#if UNIX_IO
        using OffsetType = off_t;
        static void CompletionCallback(sigval value) noexcept;
        std::unique_ptr<aiocb> _control = std::make_unique<aiocb>();
#elif WINDOWS_IO
        using OffsetType = std::uint64_t;
        static DWORD WINAPI CompletionThread(void* parameter) noexcept;
        OVERLAPPED _overlapped{};
#endif

        const NativeHandle& _nativeHandle;
        const std::variant<std::span<std::byte>, std::span<const std::byte>> _buffer;
        const OffsetType _offset;
        const OperationType _operationType{};
    };
}

#endif