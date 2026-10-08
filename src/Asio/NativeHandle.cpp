#include <utility>

#include <ExtendedCpp/Asio/NativeHandle.h>

#ifdef UNIX_IO
    #include <unistd.h>
    #include <fcntl.h>
#endif

ExtendedCpp::Asio::NativeHandle::NativeHandle(const std::string& filename, std::ios_base::openmode mode) noexcept
{
#ifdef UNIX_IO
    int flags{};

    if (mode & std::ios_base::in && mode & std::ios_base::out)
        flags |= O_RDWR;
    else if (mode & std::ios_base::in)
        flags |= O_RDONLY;
    else
        flags |= O_WRONLY;

    if (mode & std::ios_base::out)
        flags |= O_CREAT;

    if (mode & std::ios_base::trunc)
        flags |= O_TRUNC;

    if (mode & std::ios_base::app)
        flags |= O_APPEND;

    _handle = open(filename.c_str(), flags, 0666);
#elif WINDOWS_IO
    DWORD access{};

    if (mode & std::ios_base::in)
        access |= GENERIC_READ;

    if (mode & std::ios_base::out)
        access |= GENERIC_WRITE;

    DWORD creationDisposition{};

    if (mode & std::ios_base::trunc)
        creationDisposition = CREATE_ALWAYS;
    else if (mode & std::ios_base::out)
        creationDisposition = OPEN_ALWAYS;
    else
        creationDisposition = OPEN_EXISTING;

    _handle = CreateFileA(
        filename.c_str(),
        access,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        creationDisposition,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        nullptr);
#endif
}

ExtendedCpp::Asio::NativeHandle::NativeHandle(NativeHandle&& other) noexcept :
    _handle(std::exchange(other._handle, INVALID_HANDLE)) {}

ExtendedCpp::Asio::NativeHandle& ExtendedCpp::Asio::NativeHandle::operator=(NativeHandle&& other) noexcept
{
    if (this == &other)
        return *this;

    _handle = std::exchange(other._handle, INVALID_HANDLE);
    return *this;
}

ExtendedCpp::Asio::NativeHandle::~NativeHandle() noexcept
{
    if (_handle == INVALID_HANDLE)
        return;

#ifdef UNIX_IO
    close(_handle);
#elif WINDOWS_IO
    CloseHandle(_handle);
#endif

    _handle = INVALID_HANDLE;
}

ExtendedCpp::Asio::NativeHandle::operator bool() const noexcept
{
    return _handle != INVALID_HANDLE;
}

ExtendedCpp::Asio::NativeHandle::HandleType ExtendedCpp::Asio::NativeHandle::operator*() const noexcept
{
    return _handle;
}