#ifndef Asio_Aifstream_H
#define Asio_Aifstream_H

#include <stdexcept>
#include <vector>
#include <filesystem>
#include <format>
#include <ios>
#include <string>
#include <cstring>

#if __APPLE__
    #define UNIX_IO 1
#elif __linux__
    #define UNIX_IO 1
#elif _WIN32
    #define WINDOWS_IO 1
#endif

#if UNIX_IO
    #include <cstdio>
    #include <aio.h>
    #include <sys/stat.h>
#elif WINDOWS_IO
    #include <windows.h>
    #include <fileapi.h>
    #include <synchapi.h>
    #include <minwinbase.h>
	#include <handleapi.h>
    #include <io.h>
    #include <fcntl.h>
#endif

#include <ExtendedCpp/Asio/Aistream.h>
#include <ExtendedCpp/Task.h>

/// @brief 
namespace ExtendedCpp::Asio
{
    /// @brief 
    /// @tparam TChar 
    template<typename TChar>
    class BasicAifstream final : public BasicAistream<TChar>
    {
    public:
        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const char* fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
#if UNIX_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = std::fopen(fileName, "r");
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::binary:
                    _file = std::fopen(fileName, "rb");
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#elif WINDOWS_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = CreateFileA(fileName, GENERIC_READ, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY | _O_TEXT);
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::binary:
                    _file = CreateFileA(fileName, GENERIC_READ, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY);
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#endif
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::string_view fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
#if UNIX_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = std::fopen(fileName.data(), "r");
                    _offset = 0;
                case std::ios_base::in | std::ios_base::binary:
                    _file = std::fopen(fileName.data(), "rb");
                    _offset = 0;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#elif WINDOWS_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = CreateFileA(fileName.data(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY | _O_TEXT);
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::binary:
                    _file = CreateFileA(fileName.data(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY);
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#endif
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
#if UNIX_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = std::fopen(fileName.c_str(), "r");
                    _offset = 0;
                case std::ios_base::in | std::ios_base::binary:
                    _file = std::fopen(fileName.c_str(), "rb");
                    _offset = 0;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#elif WINDOWS_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = CreateFileA(fileName.c_str(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY | _O_TEXT);
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::binary:
                    _file = CreateFileA(fileName.c_str(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY);
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
#endif
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::filesystem::path& fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
#if UNIX_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = std::fopen(fileName.string().c_str(), "r");
                    _offset = 0;
                case std::ios_base::in | std::ios_base::binary:
                    _file = std::fopen(fileName.string().c_str(), "rb");
                    _offset = 0;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName.string()));
#elif WINDOWS_IO
            switch (mode)
            {
                case std::ios_base::in:
                    _file = CreateFileA(fileName.string().c_str(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY | _O_TEXT);
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::binary:
                    _file = CreateFileA(fileName.string().c_str(), GENERIC_READ, 0, NULL,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                    _fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_RDONLY);
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName.string()));
#endif
        }

        /// @brief 
        /// @param other 
        BasicAifstream(const BasicAifstream& other) = delete;

        /// @brief 
        /// @param other 
        BasicAifstream(BasicAifstream&& other) noexcept
        {
            _file = other._file;
            other._file = nullptr;
            other._fileDescriptor = -1;
            _offset = other._offset;
        }
        
        /// @brief 
        virtual ~BasicAifstream() 
        {
#if UNIX_IO
            if (_file != nullptr)
                std::fclose(_file);
#elif WINDOWS_IO
            if (_fileDescriptor != -1)
                _close(_fileDescriptor);
            else if (_file != nullptr)
                CloseHandle(_file);
#endif
        }

        /// @brief 
        /// @param other 
        /// @return 
        BasicAifstream& operator=(const BasicAifstream& other) = delete;

        /// @brief 
        /// @param other 
        /// @return 
        BasicAifstream& operator=(BasicAifstream&& other) noexcept
        {
            _file = other._file;
            other._file = nullptr;
            other._fileDescriptor = -1;
            _offset = other._offset;
            return *this;
        }

        Task<void> operator>>(std::vector<TChar>& buffer) override
        {
            buffer = co_await ReadAllAsync();
        }

        /// @brief 
        /// @param count 
        /// @return 
        Task<std::vector<TChar>> ReadAsync(std::size_t count)
        {
#ifdef UNIX_IO
            return Task<std::vector<TChar>>::Run([this](std::size_t count)
            {
                std::vector<TChar> buffer(count);

                aiocb controlBlock;
                controlBlock.aio_fildes = fileno(_file);
                controlBlock.aio_offset = _offset;
                controlBlock.aio_buf = buffer.data();
                controlBlock.aio_nbytes = count * sizeof(TChar);
                controlBlock.aio_lio_opcode = LIO_NOWAIT;

                if (aio_read(&controlBlock) == -1)
                    throw std::runtime_error(std::string("Error at aio_read().") + std::string(strerror(errno)));

                while (aio_error(&controlBlock) == EINPROGRESS) 
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));

                if (aio_error(&controlBlock) != 0)
                    throw std::runtime_error(std::string("Error at aio_write(). ") + std::string(strerror(errno)));

                const ssize_t bytesRead = aio_return(&controlBlock);
                if (bytesRead == -1) 
                    throw std::runtime_error(std::string("Error at aio_read().") + std::string(strerror(errno)));

                buffer.resize(bytesRead / sizeof(TChar));
                _offset += bytesRead;
                return buffer;
            }, count);
#elif WINDOWS_IO
            return Task<std::vector<TChar>>::Run([this](const std::size_t count)
			{
				std::vector<TChar> buffer(count);

				OVERLAPPED fileReadOverlapped;
				memset(&fileReadOverlapped, 0, sizeof(OVERLAPPED));
                fileReadOverlapped.Offset = _offset;

				BOOL readFileResult = ReadFileEx(_file, buffer.data(), static_cast<DWORD>(count * sizeof(TChar)), &fileReadOverlapped, NULL);
				if (readFileResult == FALSE)
					throw std::runtime_error(std::string("Error at ReadFileEx()."));

				DWORD waitResult = WaitForSingleObjectEx(_file, INFINITE, FALSE);
				if (waitResult != WAIT_OBJECT_0)
					throw std::runtime_error(std::string("Error at WaitForSingleObjectEx()."));

                _offset += static_cast<off_t>(count * sizeof(TChar));
				return buffer;
			}, count);
#endif
        }

        /// @brief 
        /// @return 
        Task<std::vector<TChar>> ReadAllAsync()
        {
#ifdef UNIX_IO
            struct stat fileStat;
            fstat(fileno(_file), &fileStat);
            const off_t size = fileStat.st_size;
            return ReadAsync(_offset);
#elif WINDOWS_IO
            return Task<std::vector<TChar>>::Run([this]
			{
				LARGE_INTEGER fileSize;
				GetFileSizeEx(_file, &fileSize);
				DWORD truncatedSize = fileSize.LowPart * sizeof(TChar);

				std::vector<TChar> buffer(truncatedSize);

				OVERLAPPED fileReadOverlapped;
				memset(&fileReadOverlapped, 0, sizeof(OVERLAPPED));
                fileReadOverlapped.Offset = _offset;

				BOOL readFileResult = ReadFileEx(_file, buffer.data(), truncatedSize, &fileReadOverlapped, NULL);
				if (readFileResult == FALSE)
					throw std::runtime_error(std::string("Error at ReadFileEx()."));

				DWORD waitResult = WaitForSingleObjectEx(_file, INFINITE, FALSE);
				if (waitResult != WAIT_OBJECT_0)
					throw std::runtime_error(std::string("Error at WaitForSingleObjectEx()."));

                _offset += truncatedSize * sizeof(TChar);
				return buffer;
			});
#endif
        }

        /// @brief 
        void ResetOffest() noexcept
        {
            _offset = 0;
        }

    private:
#if UNIX_IO
        FILE* _file = nullptr;
#elif WINDOWS_IO
        HANDLE _file = nullptr;
        int _fileDescriptor = -1;
#endif

        off_t _offset = 0;
    };

    typedef BasicAifstream<char> Aifstream;
}

#endif