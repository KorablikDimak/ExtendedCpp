#ifndef Asio_Aofstream_H
#define Asio_Aofstream_H

#include <stdexcept>
#include <vector>
#include <filesystem>
#include <format>
#include <ios>
#include <string>
#include <cstring>
#include <mutex>

#include <ExtendedCpp/Asio/Aostream.h>
#include <ExtendedCpp/Task.h>

#if UNIX_IO
	#include <cstdio>
	#include <aio.h>
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
	class BasicAofstream final : public BasicAostream<TChar>
	{
	public:
		/// @brief 
		/// @param fileName 
		/// @param mode 
		explicit BasicAofstream(const char* fileName, std::ios_base::openmode mode = std::ios_base::out)
		{
#if UNIX_IO
			switch (mode)
			{
				case std::ios_base::out:
					_file = std::fopen(fileName, "w");
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = std::fopen(fileName, "wb");
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = std::fopen(fileName, "a");
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = std::fopen(fileName, "ab");
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
				case std::ios_base::out:
					_file = CreateFileA(fileName, GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = CreateFileA(fileName, GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = CreateFileA(fileName, GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = CreateFileA(fileName, GENERIC_WRITE, 0, NULL,
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
		explicit BasicAofstream(const std::string_view fileName, std::ios_base::openmode mode = std::ios_base::out)
		{
#if UNIX_IO
			switch (mode)
			{
				case std::ios_base::out:
					_file = std::fopen(fileName.data(), "w");
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = std::fopen(fileName.data(), "wb");
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = std::fopen(fileName.data(), "a");
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = std::fopen(fileName.data(), "ab");
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
				case std::ios_base::out:
					_file = CreateFileA(fileName.data(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = CreateFileA(fileName.data(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = CreateFileA(fileName.data(), GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = CreateFileA(fileName.data(), GENERIC_WRITE, 0, NULL,
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
		explicit BasicAofstream(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::out)
		{
#if UNIX_IO
			switch (mode)
			{
				case std::ios_base::out:
					_file = std::fopen(fileName.c_str(), "w");
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = std::fopen(fileName.c_str(), "wb");
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = std::fopen(fileName.c_str(), "a");
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = std::fopen(fileName.c_str(), "ab");
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
				case std::ios_base::out:
					_file = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
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
		explicit BasicAofstream(const std::filesystem::path& fileName, std::ios_base::openmode mode = std::ios_base::out)
		{
#if UNIX_IO
			switch (mode)
			{
				case std::ios_base::out:
					_file = std::fopen(fileName.string().c_str(), "w");
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = std::fopen(fileName.string().c_str(), "wb");
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = std::fopen(fileName.string().c_str(), "a");
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = std::fopen(fileName.string().c_str(), "ab");
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
				case std::ios_base::out:
					_file = CreateFileA(fileName.string().c_str(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::binary:
				case std::ios_base::out | std::ios_base::binary:
					_file = CreateFileA(fileName.string().c_str(), GENERIC_WRITE, 0, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_offset = 0;
					break;
				case std::ios_base::app:
				case std::ios_base::out | std::ios_base::app:
					_file = CreateFileA(fileName.string().c_str(), GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_APPEND | _O_TEXT);
					_offset = 0;
					break;
				case std::ios_base::app | std::ios_base::binary:
				case std::ios_base::out | std::ios_base::app | std::ios_base::binary:
					_file = CreateFileA(fileName.string().c_str(), GENERIC_WRITE, 0, NULL,
						FILE_APPEND_DATA, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
					_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(_file), _O_TEXT);
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
		BasicAofstream(const BasicAofstream& other) = delete;

		/// @brief 
		/// @param other 
		BasicAofstream(BasicAofstream&& other) noexcept
		{
			std::lock_guard lock(_mutex);
			_file = other._file;
			other._file = nullptr;
			_offset = other._offset;
		}
		
		/// @brief 
		~BasicAofstream() 
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
		BasicAofstream& operator=(const BasicAofstream& other) = delete;

		/// @brief 
		/// @param other 
		/// @return 
		BasicAofstream& operator=(BasicAofstream&& other) noexcept
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

		/// @brief 
		/// @param buffer 
		/// @return 
		Task<std::size_t> WriteAsync(std::vector<TChar> buffer)
		{
#ifdef UNIX_IO
			return Task<std::size_t>::Run([this](std::vector<TChar> buffer)
			{
				std::lock_guard lock(_mutex);
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
				return static_cast<std::size_t>(bytesWrited) / sizeof(TChar);
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

	typedef BasicAofstream<char> Aofstream;
}

#endif