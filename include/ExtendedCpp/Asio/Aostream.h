#ifndef Asio_Aostream_H
#define Asio_Aostream_H

#include <vector>
#include <bit>
#include <type_traits>
#include <istream>

#include <ExtendedCpp/Asio/Aio.h>
#include <ExtendedCpp/Task.h>
#include <ExtendedCpp/Asio/Aistream.h>

/// @brief 
namespace ExtendedCpp::Asio
{
	/// @brief 
	/// @tparam TChar 
	template<typename TChar>
	class BasicAostream : virtual public BasicAio<TChar>
	{
	public:
		/// @brief 
		virtual ~BasicAostream() = default;

		/// @brief 
		/// @param  
		/// @return 
		virtual Task<void> operator<<(const std::vector<TChar>&) = 0;
	};

	/// @brief 
	typedef BasicAostream<char> Aostream;

	template<typename TChar>
	class BasicAistream;

	/// @brief 
	/// @tparam TTarget 
	/// @tparam TAostream 
	/// @tparam TChar 
	/// @param aostream 
	/// @param target 
	/// @return 
	template<typename TAostream, typename TTarget, typename TChar = TAostream::char_type>
	requires (std::is_base_of_v<BasicAostream<TChar>, TAostream> &&
			 !std::is_base_of_v<BasicAistream<TChar>, TTarget> &&
			 !std::is_base_of_v<std::basic_istream<TChar>, TTarget>)
	Task<void> operator<<(TAostream& aostream, const TTarget& target)
	{
		std::vector<TChar> buffer(sizeof(TTarget) / sizeof(TChar));
		const TChar* data = std::bit_cast<const TChar*>(&target);
		for (std::size_t i = 0; i < buffer.size(); ++i)
			buffer[i] = data[i];
		co_await(aostream << buffer);
	}

	/// @brief 
	/// @tparam TAostream 
	/// @tparam TChar 
	/// @param aostream 
	/// @param aistream 
	/// @return 
	template<typename TAostream, typename TChar = TAostream::char_type>
	requires std::is_base_of_v<BasicAostream<TChar>, TAostream>
	Task<void> operator<<(TAostream& aostream, BasicAistream<TChar>& aistream)
	{
		std::vector<TChar> buffer;
		co_await(aistream >> buffer);
		co_await(aostream << buffer);
	}

	/// @brief 
	/// @tparam TAostream 
	/// @tparam TChar 
	/// @param aostream 
	/// @param istream 
	/// @return 
	template<typename TAostream, typename TChar = TAostream::char_type>
	requires std::is_base_of_v<BasicAostream<TChar>, TAostream>
	Task<void> operator<<(TAostream& aostream, std::basic_istream<TChar>& istream)
	{
		std::vector<TChar> buffer(istream.rdbuf()->in_avail());
		istream.read(buffer.data(), buffer.size());
		co_await(aostream << buffer);
	}

	/// @brief 
	/// @tparam TAostream 
	/// @tparam TChar 
	/// @param istream 
	/// @param aostream 
	/// @return 
	template<typename TAostream, typename TChar = TAostream::char_type>
	requires std::is_base_of_v<BasicAostream<TChar>, TAostream>
	Task<void> operator>>(std::basic_istream<TChar>& istream, TAostream& aostream)
	{
		std::vector<TChar> buffer(istream.rdbuf()->in_avail());
		istream.read(buffer.data(), buffer.size());
		co_await(aostream << buffer);
	}
}

#endif