#ifndef Asio_Aifstream_H
#define Asio_Aifstream_H

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <fstream>

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
    class BasicAifstream final
    {
    public:
        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const char* fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
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
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::string_view fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
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
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
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
        }

        /// @brief 
        /// @param fileName 
        /// @param mode 
        explicit BasicAifstream(const std::filesystem::path& fileName, std::ios_base::openmode mode = std::ios_base::in)
        {
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
                throw std::invalid_argument(std::format("Cannot open file {}", fileName.string()));
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
            _offset = other._offset;
        }
        
        /// @brief 
        ~BasicAifstream() 
        {
            if (_file != nullptr)
                std::fclose(_file);
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
            _offset = other._offset;
            return *this;
        }

        template<typename TTarget>
        BasicAifstream& operator>>(TTarget& target)
        {
            const std::vector<TChar> buffer = [this]()->Task<std::vector<TChar>>
            {
                co_return co_await ReadAsync(sizeof(TTarget));
            }().Result();

            if (buffer.size() != sizeof(TTarget))
                throw std::runtime_error("BasicAifstream::operator>> error: cannot convert to target type.");

            target = *std::bit_cast<TTarget*>(buffer.data());
        }

        BasicAifstream& operator>>(std::vector<TChar>& buffer)
        {
            buffer = [this]()->Task<std::vector<TChar>>
            {
                co_return co_await ReadAllAsync();
            }().Result();
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
            return ReadAsync(size - _offset);
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

    typedef BasicAifstream<char> Aifstream;
}

#endif