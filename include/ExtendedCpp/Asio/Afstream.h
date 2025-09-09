#ifndef Asio_Afstream_H
#define Asio_Afstream_H

#include <stdexcept>
#include <vector>
#include <filesystem>
#include <format>
#include <ios>
#include <string>
#include <cstring>
#include <mutex>

#include <ExtendedCpp/Asio/Astream.h>
#include <ExtendedCpp/Task.h>

#if UNIX_IO
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
	#include <sys/types.h>
#endif

/// @brief 
namespace ExtendedCpp::Asio
{
	/// @brief 
	/// @tparam TChar 
	template<typename TChar>
	class BasicAfstream final : public BasicAstream<TChar>
	{
	public:
		/// @brief 
		/// @param fileName 
		/// @param mode 
		explicit BasicAfstream(const char* fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
		{
#if UNIX_IO
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
#elif WINDOWS_IO
			switch (mode)
			{
				case std::ios_base::in | std::ios_base::out:
					_file = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
					_file = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::in | std::ios_base::out | std::ios_base::app:
					_file = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND);
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
		explicit BasicAfstream(const std::string_view fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
		{
#if UNIX_IO
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
#elif WINDOWS_IO
			switch (mode)
			{
			case std::ios_base::in | std::ios_base::out:
				_file = CreateFileA(fileName.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
				_offset = 0;
				break;
			case std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
				_file = CreateFileA(fileName.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_offset = 0;
				break;
			case std::ios_base::app:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app:
				_file = CreateFileA(fileName.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
				_offset = 0;
				break;
			case std::ios_base::app | std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
				_file = CreateFileA(fileName.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND);
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
		explicit BasicAfstream(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
		{
#if UNIX_IO
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
#elif WINDOWS_IO
			switch (mode)
			{
			case std::ios_base::in | std::ios_base::out:
				_file = CreateFileA(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_offset = 0;
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
				break;
			case std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
				_file = CreateFileA(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_offset = 0;
				break;
			case std::ios_base::app:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app:
				_file = CreateFileA(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
				_offset = 0;
				break;
			case std::ios_base::app | std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
				_file = CreateFileA(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND);
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
		explicit BasicAfstream(const std::filesystem::path& fileName, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
		{
#if UNIX_IO
			switch (mode)
			{
				case std::ios_base::in | std::ios_base::out:
					_file = std::fopen(fileName.string().c_str(), "w+");
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
					_file = std::fopen(fileName.string().c_str(), "w+b");
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::in | std::ios_base::out | std::ios_base::app:
					_file = std::fopen(fileName.string().c_str(), "a+");
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = std::fopen(fileName.string().c_str(), "a+b");
					_offset = 0;
					break;
				default:
					throw std::invalid_argument("Incorrect openmode.");
			}

			if (_file == nullptr)
				throw std::invalid_argument(std::format("Cannot open file {}", fileName.string()));
#elif WINDOWS_IO
			switch (mode)
			{
			case std::ios_base::in | std::ios_base::out:
				_file = CreateFileA(fileName.string().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
				_offset = 0;
				break;
			case std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::binary:
				_file = CreateFileA(fileName.string().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_offset = 0;
				break;
			case std::ios_base::app:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app:
				_file = CreateFileA(fileName.string().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
				_offset = 0;
				break;
			case std::ios_base::app | std::ios_base::binary:
			case std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary:
				_file = CreateFileA(fileName.string().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND);
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
		BasicAfstream(const BasicAfstream& other) = delete;

		/// @brief 
		/// @param other 
		BasicAfstream(BasicAfstream&& other) noexcept
		{
			std::lock_guard lock(_mutex);
			_file = other._file;
			other._file = nullptr;
			_offset = other._offset;
		}
		
		/// @brief 
		~BasicAfstream() override
		{
#if UNIX_IO
			if (_file != nullptr)
			{
				std::fclose(_file);
				_file = nullptr;
			}
#elif WINDOWS_IO
			if (_fileDescriptor != -1)
			{
				_close(_fileDescriptor);
				_fileDescriptor = -1;
			}
			else if (_file != nullptr)
			{
				CloseHandle(_file);
				_file = nullptr;
			}
#endif
		}

		/// @brief 
		void Close()
		{
			std::lock_guard lock(_mutex);
#if UNIX_IO
			if (_file != nullptr)
			{
				std::fclose(_file);
				_file = nullptr;
			}
#elif WINDOWS_IO
			if (_fileDescriptor != -1)
			{
				_close(_fileDescriptor);
				_fileDescriptor = -1;
			}
			else if (_file != nullptr)
			{
				CloseHandle(_file);
				_file = nullptr;
			}
#endif
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
			std::lock_guard lock(_mutex);
			_file = other._file;
			other._file = nullptr;
			_offset = other._offset;
			return *this;
		}

		Task<void> operator<<(const std::vector<TChar>& buffer) override
		{
			co_await WriteAsync(buffer);
		}

		Task<void> operator>>(std::vector<TChar>& buffer) override
		{
			buffer = co_await ReadAllAsync();
		}

		/// @brief 
		/// @param count 
		/// @return 
		Task<std::vector<TChar>> ReadAsync(const std::size_t count)
		{
#if UNIX_IO
			return Task<std::vector<TChar>>::Run([this, count]
			{
				std::lock_guard lock(_mutex);
				std::vector<TChar> buffer(count);

				aiocb controlBlock{};
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
			});
#elif WINDOWS_IO
			return Task<std::vector<TChar>>::Run([this](const std::size_t count)
			{
				std::lock_guard lock(_mutex);
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

				return buffer;
			}, count);
#endif
		}

		/// @brief 
		/// @return 
		Task<std::vector<TChar>> ReadAllAsync()
		{
#if UNIX_IO
			struct stat fileStat{};
			fstat(fileno(_file), &fileStat);
			const off_t size = fileStat.st_size;
			return ReadAsync(size - _offset);
#elif WINDOWS_IO
			return Task<std::vector<TChar>>::Run([this]
			{
				std::lock_guard lock(_mutex);
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
		/// @param buffer 
		/// @return 
		Task<std::size_t> WriteAsync(std::vector<TChar> buffer)
		{
#if UNIX_IO
			return Task<std::size_t>::Run([this](std::vector<TChar> buffer)
			{
				std::lock_guard lock(_mutex);
				aiocb controlBlock{};
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
				return bytesWrited / sizeof(TChar);
			}, std::move(buffer));
#elif WINDOWS_IO
			return Task<std::size_t>::Run([this](std::vector<TChar> buffer)
			{
				std::lock_guard lock(_mutex);
				OVERLAPPED fileReadOverlapped;
				memset(&fileReadOverlapped, 0, sizeof(OVERLAPPED));
				fileReadOverlapped.Offset = _offset;

				BOOL writeFileResult = WriteFileEx(_file, buffer.data(), static_cast<DWORD>(buffer.size() * sizeof(TChar)), &fileReadOverlapped, NULL);
				if (writeFileResult == FALSE)
					throw std::runtime_error(std::string("Error at WriteFileEx()."));

				DWORD waitResult = WaitForSingleObjectEx(_file, INFINITE, FALSE);
				if (waitResult != WAIT_OBJECT_0)
					throw std::runtime_error(std::string("Error at WaitForSingleObjectEx()."));

				return buffer.size();
			}, std::move(buffer));
#endif
		}

		/// @brief 
		void ResetOffest() noexcept
		{
			std::lock_guard lock(_mutex);
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
		mutable std::mutex _mutex;
	};

	typedef BasicAfstream<char> Afstream;
}

#endif