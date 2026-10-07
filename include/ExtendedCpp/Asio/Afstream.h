#ifndef Asio_Afstream_H
#define Asio_Afstream_H

#include <fstream>

#include <ExtendedCpp/Task.h>
#include <ExtendedCpp/Asio/FileIoAwaiter.h>

namespace ExtendedCpp::Asio
{
    class Afstream final : public std::fstream
    {
    public:
        explicit Afstream(const std::string& filename, openmode mode = in | out);

        Task<std::vector<std::byte>> ReadAsync(std::streamsize count);
        Task<std::streamsize> WriteAsync(std::span<std::byte> buffer);

    private:
        std::uint64_t _position = 0;
        NativeHandle _nativeHandle;
    };
}

#endif