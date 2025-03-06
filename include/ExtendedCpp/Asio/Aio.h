#ifndef Asio_Aio_H
#define Asio_Aio_H

#if __APPLE__
	#define UNIX_IO 1
#elif __linux__
	#define UNIX_IO 1
#elif _WIN32
	#define WINDOWS_IO 1
#endif

/// @brief 
namespace ExtendedCpp::Asio
{
	/// @brief 
	template<typename TChar>
	class BasicAio
	{
	public:
		/// @brief 
		using char_type = TChar;

		/// @brief 
		virtual ~BasicAio() = default;
	};
}

#endif