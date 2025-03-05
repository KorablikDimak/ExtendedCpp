#ifndef Asio_Aostream_H
#define Asio_Aostream_H

#include <vector>
#include <bit>

#include <ExtendedCpp/Asio/Aio.h>
#include <ExtendedCpp/Task.h>

namespace ExtendedCpp::Asio
{
	template<typename TChar>
	class BasicAostream : virtual public BasicAio<TChar>
	{
	public:
		virtual ~BasicAostream() = default;

		virtual Task<void> operator<<(const std::vector<TChar>&) = 0;

		template<typename TTarget>
		Task<void> operator<<(const TTarget& target)
		{
			std::vector<TChar> buffer(sizeof(TTarget) / sizeof(TChar));
			const TChar* data = std::bit_cast<TChar*>(&target);
			for (std::size_t i = 0; i < buffer.size(); ++i)
				buffer[i] = data[i];
			co_await(*this << buffer);
		}
	};

	typedef BasicAostream<char> Aostream;
}

#endif