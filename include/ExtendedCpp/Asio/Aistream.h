#ifndef Asio_Aistream_H
#define Asio_Aistream_H

#include <vector>
#include <bit>
#include <stdexcept>
#include <type_traits>
#include <ostream>

#include <ExtendedCpp/Asio/Aio.h>
#include <ExtendedCpp/Task.h>
#include <ExtendedCpp/Asio/Aostream.h>

/// @brief 
namespace ExtendedCpp::Asio
{
	/// @brief 
	/// @tparam TChar 
	template<typename TChar>
	class BasicAistream : virtual public BasicAio<TChar>
	{
	public:
		/// @brief 
		~BasicAistream() override = default;

		/// @brief 
		/// @param  
		/// @return 
		virtual Task<void> operator>>(std::vector<TChar>&) = 0;
	};

	/// @brief 
	typedef BasicAistream<char> Aistream;

	template<typename TChar>
	class BasicAostream;

	/// @brief 
	/// @tparam TAistream 
	/// @tparam TTarget 
	/// @tparam TChar 
	/// @param aistream 
	/// @param target 
	/// @return 
	template<typename TAistream, typename TTarget, typename TChar = typename TAistream::char_type>
	requires (std::is_base_of_v<BasicAistream<TChar>, TAistream> &&
			 !std::is_base_of_v<BasicAostream<TChar>, TTarget> &&
			 !std::is_base_of_v<std::basic_ostream<TChar>, TTarget>)
	Task<void> operator>>(TAistream& aistream, TTarget& target)
	{
		std::vector<TChar> buffer;
		co_await(aistream >> buffer);
		if (buffer.size() != sizeof(TTarget) / sizeof(TChar))
			throw std::runtime_error("BasicAifstream::operator>> error: cannot convert to target type.");
		target = *std::bit_cast<const TTarget*>(buffer.data());
	}

	/// @brief 
	/// @tparam TAistream 
	/// @tparam TChar 
	/// @param aistream 
	/// @param aostream 
	/// @return 
	template<typename TAistream, typename TChar = typename TAistream::char_type>
	requires std::is_base_of_v<BasicAistream<TChar>, TAistream>
	Task<void> operator>>(TAistream& aistream, BasicAostream<TChar>& aostream)
	{
		std::vector<TChar> buffer;
		co_await(aistream >> buffer);
		co_await(aostream << buffer);
	}

	/// @brief 
	/// @tparam TAistream 
	/// @tparam TChar 
	/// @param aistream 
	/// @param ostream 
	/// @return 
	template<typename TAistream, typename TChar = typename TAistream::char_type>
	requires std::is_base_of_v<BasicAistream<TChar>, TAistream>
	Task<void> operator>>(TAistream& aistream, std::basic_ostream<TChar>& ostream)
	{
		std::vector<TChar> buffer;
		co_await(aistream >> buffer);
		ostream.write(buffer.data(), buffer.size());
	}

	/// @brief 
	/// @tparam TAistream 
	/// @tparam TChar 
	/// @param ostream 
	/// @param aistream 
	/// @return 
	template<typename TAistream, typename TChar = typename TAistream::char_type>
	requires std::is_base_of_v<BasicAistream<TChar>, TAistream>
	Task<void> operator<<(std::basic_ostream<TChar>& ostream, TAistream& aistream)
	{
		std::vector<TChar> buffer;
		co_await(aistream >> buffer);
		ostream.write(buffer.data(), buffer.size());
	}
}

#endif