#include <format>
#include <algorithm>
#include <array>
#include <utility>

#include <ExtendedCpp/Asio/Afstream.h>
#include <ExtendedCpp/Asio/ReadFileAwaiter.h>
#include <ExtendedCpp/Asio/WriteFileAwaiter.h>

ExtendedCpp::Asio::Afstream::Afstream(const std::string& filename, const openmode mode) :
    std::fstream(filename, mode),
    _nativeHandle(NativeHandle(filename, mode))
{
    if (!is_open() || !_nativeHandle)
        throw failure(std::format("Cannot open file '{}'", filename));
}

ExtendedCpp::Asio::Afstream::Afstream(Afstream&& other) noexcept :
    std::fstream(std::move(other)),
    _readPosition(other._readPosition),
    _writePosition(other._writePosition),
    _nativeHandle(std::move(other._nativeHandle)) {}

ExtendedCpp::Asio::Afstream& ExtendedCpp::Asio::Afstream::operator=(Afstream&& other) noexcept
{
    if (this == &other)
        return *this;

    std::fstream::operator=(std::move(other));

    _readPosition = other._readPosition;
    _writePosition = other._writePosition;
    _nativeHandle = std::move(other._nativeHandle);
    return *this;
}

ExtendedCpp::Task<std::vector<std::byte>> ExtendedCpp::Asio::Afstream::ReadAsync(const std::size_t count)
{
    std::vector<std::byte> buffer(count);

    const OffsetType position = GetReadPosition();

    ReadFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        std::span(buffer)
    };

    const std::streamsize read = co_await ioOperation;

    if (read <= 0)
        co_return {};

    buffer.resize(read);
    SetReadPosition(position + read);
    co_return buffer;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::ReadAsync(const std::span<std::byte> buffer)
{
    const OffsetType position = GetReadPosition();

    ReadFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        buffer
    };

    const std::streamsize read = co_await ioOperation;

    if (read > 0)
        SetReadPosition(position + read);

    co_return read;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::ReadAsync(
    const std::span<std::byte> buffer,
    const std::size_t count)
{
    const OffsetType position = GetReadPosition();

    ReadFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        buffer.first((std::min)(count, buffer.size()))
    };

    const std::streamsize read = co_await ioOperation;

    if (read > 0)
        SetReadPosition(position + read);

    co_return read;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::span<const std::byte> buffer)
{
    const OffsetType position = GetWritePosition();

    WriteFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        buffer
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        SetWritePosition(position + written);

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(
    const std::span<const std::byte> buffer,
    const std::size_t count)
{
    const OffsetType position = GetWritePosition();

    WriteFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        buffer.first((std::min)(count, buffer.size()))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        SetWritePosition(position + written);

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(const std::string_view buffer)
{
    const OffsetType position = GetWritePosition();

    WriteFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        std::as_bytes(std::span(buffer))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        SetWritePosition(position + written);

    co_return written;
}

ExtendedCpp::Task<std::streamsize> ExtendedCpp::Asio::Afstream::WriteAsync(
    const std::string_view buffer,
    const std::size_t count)
{
    const OffsetType position = GetWritePosition();

    WriteFileAwaiter ioOperation // NOLINT(*-const-correctness)
    {
        _nativeHandle,
        position,
        std::as_bytes(std::span(buffer)).first((std::min)(count, buffer.size()))
    };

    const std::streamsize written = co_await ioOperation;

    if (written > 0)
        SetWritePosition(position + written);

    co_return written;
}

ExtendedCpp::Task<std::vector<std::byte>> ExtendedCpp::Asio::Afstream::ReadAllAsync()
{
    std::vector<std::byte> result;

    constexpr std::size_t chunkSize = 4096;
    std::array<std::byte, chunkSize> chunk{};

    OffsetType currentPosition = GetReadPosition();

    while (true)
    {
        ReadFileAwaiter ioOperation // NOLINT(*-const-correctness)
        {
            _nativeHandle,
            currentPosition,
            std::span<std::byte>(chunk)
        };

        const std::streamsize read = co_await ioOperation;

        if (read > 0 && static_cast<std::size_t>(read) < chunk.size())
            currentPosition += read;
        else
            break;

        result.insert(result.end(), chunk.begin(), chunk.begin() + read);
    }

    SetReadPosition(currentPosition);

    co_return result;
}

ExtendedCpp::Task<std::string> ExtendedCpp::Asio::Afstream::ReadLineAsync()
{
    std::string result;

    constexpr std::size_t chunkSize = 256;
    std::array<std::byte, chunkSize> chunk{};

    OffsetType currentPosition = GetReadPosition();

    while (true)
    {
        ReadFileAwaiter ioOperation // NOLINT(*-const-correctness)
        {
            _nativeHandle,
            currentPosition,
            std::span<std::byte>(chunk)
        };

        const std::streamsize read = co_await ioOperation;

        if (read > 0 && static_cast<std::size_t>(read) < chunk.size())
            currentPosition += read;
        else
            break;

        const std::byte* endOfLine = std::find(chunk.begin(), chunk.begin() + read, std::byte{'\n'});
        const std::size_t lineLength = std::distance(chunk.cbegin(), endOfLine);

        result.append(reinterpret_cast<char*>(chunk.data()), lineLength);

        if (std::cmp_less(lineLength, read))
        {
            currentPosition -= read - static_cast<OffsetType>(lineLength) - 1;
            break;
        }
    }

    SetReadPosition(currentPosition);

    co_return result;
}

ExtendedCpp::Task<std::string> ExtendedCpp::Asio::Afstream::ReadTextAsync()
{
    const std::vector<std::byte> bytes = co_await ReadAllAsync();

    if (bytes.empty())
        co_return {};

    co_return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

ExtendedCpp::Asio::OffsetType ExtendedCpp::Asio::Afstream::GetReadPosition()
{
    const std::int64_t syncReadPosition = tellg();
    if (syncReadPosition == -1)
        throw failure("tellg() failed");

    _readPosition = syncReadPosition;
    return syncReadPosition;
}

void ExtendedCpp::Asio::Afstream::SetReadPosition(const OffsetType position)
{
    _readPosition = position;
    seekg(position);
}

ExtendedCpp::Asio::OffsetType ExtendedCpp::Asio::Afstream::GetWritePosition()
{
    const std::int64_t syncWritePosition = tellp();
    if (syncWritePosition == -1)
        throw failure("tellp() failed");

    _readPosition = syncWritePosition;
    return syncWritePosition;
}

void ExtendedCpp::Asio::Afstream::SetWritePosition(const OffsetType position)
{
    _writePosition = position;
    seekp(position);
}