#ifndef Asio_Aistream_H
#define Asio_Aistream_H

#include <vector>
#include <bit>
#include <stdexcept>

#include <ExtendedCpp/Asio/Aio.h>
#include <ExtendedCpp/Task.h>

namespace ExtendedCpp::Asio
{
	template<typename TChar>
	class BasicAistream : virtual public BasicAio<TChar>
	{
	public:
		virtual ~BasicAistream() = default;

		virtual Task<void> operator>>(std::vector<TChar>&) = 0;

		template<typename TTarget>
		Task<void> operator>>(TTarget& target)
		{
			std::vector<TChar> buffer;
			co_await (*this >> buffer);
			if (buffer.size() != sizeof(TTarget) / sizeof(TChar))
				throw std::runtime_error("BasicAifstream::operator>> error: cannot convert to target type.");
			target = *std::bit_cast<TTarget*>(buffer.data());
		}
	};

	typedef BasicAistream<char> Aistream;
}

#endif