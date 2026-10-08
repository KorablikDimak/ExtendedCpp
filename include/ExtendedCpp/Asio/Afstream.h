#ifndef Asio_Afstream_H
#define Asio_Afstream_H

#include <fstream>
#include <vector>

#include <ExtendedCpp/Task.h>
#include <ExtendedCpp/Asio/FileIoAwaiter.h>

namespace ExtendedCpp::Asio
{
    class Afstream final : public std::fstream
    {
    public:
        explicit Afstream(const std::string& filename, openmode mode = in | out);

        Afstream(const Afstream&) noexcept = delete;
        Afstream& operator=(const Afstream&) noexcept = delete;

        Afstream(Afstream&& other) noexcept;
        Afstream& operator=(Afstream&& other) noexcept;

        ~Afstream() noexcept override = default;

        Task<std::vector<std::byte>> ReadAsync(std::size_t count);
        Task<std::streamsize> ReadAsync(std::span<std::byte> buffer);
        Task<std::streamsize> ReadAsync(std::span<std::byte> buffer, std::size_t count);

        Task<std::streamsize> WriteAsync(std::span<const std::byte> buffer);
        Task<std::streamsize> WriteAsync(std::span<const std::byte> buffer, std::size_t count);
        Task<std::streamsize> WriteAsync(std::string_view buffer);
        Task<std::streamsize> WriteAsync(std::string_view buffer, std::size_t count);

        Task<std::vector<std::byte>> ReadAllAsync();
        Task<std::streamsize> ReadAllAsync(std::span<std::byte> buffer);

        Task<std::string> ReadLineAsync();
        Task<std::string> ReadTextAsync();

    private:
        std::uint64_t _position = 0;
        NativeHandle _nativeHandle;
    };
}

#endif