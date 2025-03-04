#ifndef Asio_Afstream_H
#define Asio_Afstream_H

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <filesystem>

#ifdef __APPLE__
    #define UNIX_IO
#elif __linux__
    #define UNIX_IO
#elif _WIN32
    #define WINDOWS_IO
#endif

#ifdef UNIX_IO
    #include <aio.h>
    #include <sys/stat.h>
#elif WINDOWS_IO
    #include <windows.h>
#endif

#include <ExtendedCpp/Task.h>

/// @brief 
namespace ExtendedCpp::Asio
{
    /// @brief 
    /// @tparam TChar 
    template<typename TChar>
    class BasicAfstream final
    {
    public:
        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAfstream(const char* fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            switch (mode)
            {
                case std::ios_base::in | std::ios_base::out:
                    _file = std::fopen(fileName, "w+");
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
                    _file = std::fopen(fileName, "w+b");
                    _offset = 0;
                    break;
                case std::ios_base::app:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app:
                    _file = std::fopen(fileName, "a+");
                    _offset = 0;
                    break;
                case std::ios_base::app | std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
                    _file = std::fopen(fileName, "a+b");
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAfstream(const std::string_view fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            switch (mode)
            {
                case std::ios_base::in | std::ios_base::out:
                    _file = std::fopen(fileName.data(), "w+");
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
                    _file = std::fopen(fileName.data(), "w+b");
                    _offset = 0;
                    break;
                case std::ios_base::app:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app:
                    _file = std::fopen(fileName.data(), "a+");
                    _offset = 0;
                    break;
                case std::ios_base::app | std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
                    _file = std::fopen(fileName.data(), "a+b");
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAfstream(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            switch (mode)
            {
                case std::ios_base::in | std::ios_base::out:
                    _file = std::fopen(fileName.c_str(), "w+");
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
                    _file = std::fopen(fileName.c_str(), "w+b");
                    _offset = 0;
                    break;
                case std::ios_base::app:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app:
                    _file = std::fopen(fileName.c_str(), "a+");
                    _offset = 0;
                    break;
                case std::ios_base::app | std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
                    _file = std::fopen(fileName.c_str(), "a+b");
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName));
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAfstream(const std::filesystem::path& fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            switch (mode)
            {
                case std::ios_base::in | std::ios_base::out:
                    _file = std::fopen(fileName.c_str(), "w+");
                    _offset = 0;
                    break;
                case std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
                    _file = std::fopen(fileName.c_str(), "w+b");
                    _offset = 0;
                    break;
                case std::ios_base::app:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app:
                    _file = std::fopen(fileName.c_str(), "a+");
                    _offset = 0;
                    break;
                case std::ios_base::app | std::ios_base::binary:
                case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
                    _file = std::fopen(fileName.c_str(), "a+b");
                    _offset = 0;
                    break;
                default:
                    throw std::invalid_argument("Incorrect openmode.");
            }

            if (_file == nullptr)
                throw std::invalid_argument(std::format("Cannot open file {}", fileName.string()));
        }

        /// @brief 
        /// @param other 
        BasicAfstream(const BasicAfstream& other) = delete;

        /// @brief 
        /// @param other 
        BasicAfstream(BasicAfstream&& other) noexcept
        {
            _file = other._file;
            other._file = nullptr;
            _offset = other._offset;
        }
        
        /// @brief 
        ~BasicAfstream()
        {
            if (_file != nullptr)
                std::fclose(_file);
        }

        /// @brief 
        /// @param other 
        /// @return 
        BasicAfstream& operator=(const BasicAfstream& other) = delete;

        /// @brief 
        /// @param other 
        /// @return 
        BasicAfstream& operator=(BasicAfstream&& other) noexcept
        {
            _file = other._file;
            other._file = nullptr;
            _offset = other._offset;
            return *this;
        }

        /// @brief 
        /// @param count 
        /// @return 
        Task<std::vector<TChar>> ReadAsync(size_t count)
        {
#ifdef UNIX_IO
            return Task<std::vector<TChar>>::Run([this](size_t count)
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
#endif
        }

        /// @brief 
        /// @return 
        Task<std::vector<TChar>> ReadAllAsync()
        {
#ifdef UNIX_IO
            struct stat fileStat;
            fstat(fileno(_file), &fileStat);
            off_t size = fileStat.st_size;
            return ReadAsync(size);
#elif WINDOWS_IO
#endif
        }

        /// @brief 
        /// @param buffer 
        /// @return 
        Task<ssize_t> WriteAsync(std::vector<TChar> buffer)
        {
#ifdef UNIX_IO
            return Task<ssize_t>::Run([this](std::vector<TChar> buffer)
            {
                aiocb controlBlock;
                memset(&controlBlock, 0, sizeof(aiocb));
                controlBlock.aio_fildes = fileno(_file);
                controlBlock.aio_offset = _offset;
                controlBlock.aio_buf = buffer.data();
                controlBlock.aio_nbytes = buffer.size() * sizeof(TChar);
                controlBlock.aio_lio_opcode = LIO_NOWAIT;

                if (aio_write(&controlBlock) == -1)
                    throw std::runtime_error(std::string("Error at aio_write(). ") + std::string(strerror(errno)));

                while (aio_error(&controlBlock) == EINPROGRESS) 
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));

                if (aio_error(&controlBlock) != 0)
                    throw std::runtime_error(std::string("Error at aio_write(). ") + std::string(strerror(errno)));

                const ssize_t bytesWrited = aio_return(&controlBlock);
                if (bytesWrited == -1) 
                    throw std::runtime_error(std::string("Error at aio_write(). ") + std::string(strerror(errno)));
    
                _offset += bytesWrited;
                return bytesWrited;
            }, buffer);
#elif WINDOWS_IO
#endif
        }

        /// @brief 
        void ResetOffest() noexcept
        {
            _offset = 0;
        }

    private:
        FILE* _file;
        off_t _offset;
    };

    typedef BasicAfstream<char> Afstream;
}

#endif