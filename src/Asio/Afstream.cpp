#include <ExtendedCpp/Asio/Afstream.h>

ExtendedCpp::Asio::Afstream::Afstream(const std::string& filename, openmode mode)
{

}

ExtendedCpp::Task<std::vector<std::byte>> ExtendedCpp::Asio::Afstream::ReadAsync(const std::streamsize count)
{
    std::vector<std::byte> buffer(count);

    FileIoAwaiter awaiter
    {
        _nativeHandle,
        _position,
        buffer,
        OperationType::Read
    };

    const std::streamsize read = co_await awaiter;
    buffer.resize(read);
    _position += read;
    co_return buffer;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::span<std::byte> buffer)
{
    FileIoAwaiter awaiter
    {
        _nativeHandle,
        _position,
        buffer,
        OperationType::Write
    };

    const std::streamsize written = co_await awaiter;
    _position += written;
    co_return written;
}