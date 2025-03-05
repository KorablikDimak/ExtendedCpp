#ifndef Asio_Astream_H
#define Asio_Astream_H

#include <ExtendedCpp/Asio/Aistream.h>
#include <ExtendedCpp/Asio/Aostream.h>

namespace ExtendedCpp::Asio
{
	template<typename TChar>
	class BasicAstream : public BasicAistream<TChar>, public BasicAostream<TChar>
	{
	public:
		virtual ~BasicAstream() = default;
	};

	typedef BasicAstream<char> Astream;
}

#endif