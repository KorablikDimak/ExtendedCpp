#ifndef Asio_Astream_H
#define Asio_Astream_H

#include <ExtendedCpp/Asio/Aistream.h>
#include <ExtendedCpp/Asio/Aostream.h>

/// @brief 
namespace ExtendedCpp::Asio
{
	/// @brief 
	/// @tparam TChar 
	template<typename TChar>
	class BasicAstream : public BasicAistream<TChar>, public BasicAostream<TChar>
	{
	public:
		/// @brief 
		~BasicAstream() override = default;
	};

	/// @brief 
	typedef BasicAstream<char> Astream;
}

#endif