#ifndef Asio_NativeHandle_H
#define Asio_NativeHandle_H

#include <string>
#include <ios>

#if __APPLE__
    #define UNIX_IO 1
#elif __linux__
    #define UNIX_IO 1
#elif _WIN32
    #define WINDOWS_IO 1
#endif

#ifdef WINDOWS_IO
    #include <windows.h>
#endif

namespace ExtendedCpp::Asio
{
    class NativeHandle final
    {
    public:
        NativeHandle(const std::string& filename, std::ios_base::openmode mode) noexcept;

        NativeHandle(const NativeHandle&) noexcept = delete;
        NativeHandle& operator=(const NativeHandle&) noexcept = delete;

        NativeHandle(NativeHandle&& other) noexcept;
        NativeHandle& operator=(NativeHandle&& other) noexcept;

        ~NativeHandle() noexcept;

        explicit operator bool() const noexcept;

    private:
#ifdef UNIX_IO
        using HandleType = int;
        static constexpr HandleType INVALID_HANDLE = -1;
#elif WINDOWS_IO
        using HandleType = HANDLE;
        inline static const auto INVALID_HANDLE = INVALID_HANDLE_VALUE;
#endif

        HandleType _handle;

    public:
        HandleType operator*() const noexcept;
    };
}

#endif