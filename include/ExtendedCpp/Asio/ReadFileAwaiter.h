#ifndef READ_FILE_AWAITER_H
#define READ_FILE_AWAITER_H

#include <span>

#include <ExtendedCpp/Asio/FileIoAwaiter.h>

namespace ExtendedCpp::Asio
{
    class ReadFileAwaiter final : public FileIoAwaiter
    {
    public:
        ReadFileAwaiter(const NativeHandle& nativeHandle, OffsetType offset, std::span<std::byte> buffer) noexcept;

        ReadFileAwaiter(const ReadFileAwaiter& other) noexcept = delete;
        ReadFileAwaiter(ReadFileAwaiter&& other) noexcept = delete;

        ReadFileAwaiter& operator=(const ReadFileAwaiter& other) noexcept = delete;
        ReadFileAwaiter& operator=(ReadFileAwaiter&& other) noexcept = delete;

        ~ReadFileAwaiter() noexcept override = default;

    protected:
        void Start() noexcept override;

    private:
        std::span<std::byte> _buffer;
    };
}

#endif