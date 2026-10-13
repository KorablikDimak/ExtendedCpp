#ifndef WRITE_FILE_AWAITER_H
#define WRITE_FILE_AWAITER_H

#include <span>

#include <ExtendedCpp/Asio/FileIoAwaiter.h>

namespace ExtendedCpp::Asio
{
    class WriteFileAwaiter final : public FileIoAwaiter
    {
    public:
        WriteFileAwaiter(const NativeHandle& nativeHandle, OffsetType offset, std::span<const std::byte> buffer) noexcept;

        WriteFileAwaiter(const WriteFileAwaiter& other) noexcept = delete;
        WriteFileAwaiter(WriteFileAwaiter&& other) noexcept = delete;

        WriteFileAwaiter& operator=(const WriteFileAwaiter& other) noexcept = delete;
        WriteFileAwaiter& operator=(WriteFileAwaiter&& other) noexcept = delete;

        ~WriteFileAwaiter() noexcept override = default;

    protected:
        void Start() noexcept override;

    private:
        std::span<const std::byte> _buffer;
    };
}

#endif