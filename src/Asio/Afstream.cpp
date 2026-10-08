#include <format>

#include <ExtendedCpp/Asio/Afstream.h>

ExtendedCpp::Asio::Afstream::Afstream(const std::string& filename, const openmode mode) :
    std::fstream(filename, mode),
    _nativeHandle(NativeHandle(filename, mode))
{
    if (!is_open() || !_nativeHandle)
        throw failure(std::format("Cannot open file '{}'", filename));
}

ExtendedCpp::Asio::Afstream::Afstream(Afstream&& other) noexcept :
    std::fstream(std::move(other)),
    _position(other._position),
    _nativeHandle(std::move(other._nativeHandle)) {}

ExtendedCpp::Asio::Afstream& ExtendedCpp::Asio::Afstream::operator=(Afstream&& other) noexcept
{
    if (this == &other)
        return *this;

    std::fstream::operator=(std::move(other));
    _position = other._position;
    _nativeHandle = std::move(other._nativeHandle);
    return *this;
}

ExtendedCpp::Task<std::vector<std::byte>> ExtendedCpp::Asio::Afstream::ReadAsync(const std::size_t count)
{
    std::vector<std::byte> buffer(count);

    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        std::span(buffer)
    };

    const std::streamsize read = co_await ioOperation;

    if (read <= 0)
        co_return {};

    buffer.resize(read);
    _position += read;
    co_return buffer;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::ReadAsync(const std::span<std::byte> buffer)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        buffer
    };

    const std::streamsize read = co_await ioOperation;

    if (read > 0)
        _position += read;

    co_return read;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::ReadAsync(const std::span<std::byte> buffer, const std::size_t count)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        buffer.first(std::min(count, buffer.size()))
    };

    const std::streamsize read = co_await ioOperation;

    if (read > 0)
        _position += read;

    co_return read;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::span<const std::byte> buffer)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        buffer
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        _position += written;

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::span<const std::byte> buffer, const std::size_t count)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        buffer.first(std::min(count, buffer.size()))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        _position += written;

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::string_view buffer)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        std::as_bytes(std::span(buffer))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        _position += written;

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::string_view buffer, const std::size_t count)
{
    FileIoAwaiter ioOperation
    {
        _nativeHandle,
        _position,
        std::as_bytes(std::span(buffer)).first(std::min(count, buffer.size()))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        _position += written;

    co_return written;
}

ExtendedCpp::Task<std::vector<std::byte>> ExtendedCpp::Asio::Afstream::ReadAllAsync()
{
    // TODO ReadAllAsync()
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::ReadAllAsync(const std::span<std::byte> buffer)
{
    // TODO ReadAllAsync(buffer)
}

ExtendedCpp::Task<std::string> ExtendedCpp::Asio::Afstream::ReadLineAsync()
{
    // TODO ReadLineAsync()
}

ExtendedCpp::Task<std::string> ExtendedCpp::Asio::Afstream::ReadTextAsync()
{
    // TODO ReadTextAsync()
}