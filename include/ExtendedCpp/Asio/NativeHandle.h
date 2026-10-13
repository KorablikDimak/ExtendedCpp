#ifndef ASIO_NATIVE_HANDLE_H
#define ASIO_NATIVE_HANDLE_H

#include <ios>
#include <string>

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
        void CloseHandle() noexcept;

#if defined(UNIX_IO) || defined(DARWIN_IO)
        using HandleType = int;
        static constexpr HandleType INVALID_HANDLE = -1;
#elif defined(WINDOWS_IO)
        using HandleType = HANDLE;
        inline static const auto INVALID_HANDLE = INVALID_HANDLE_VALUE;
#endif

        HandleType _handle;

    public:
        HandleType operator*() const noexcept;
    };
}

#endif