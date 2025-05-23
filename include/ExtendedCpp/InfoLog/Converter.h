#ifndef InfoLog_Converter_H
#define InfoLog_Converter_H

#include <algorithm>

#include <ExtendedCpp/InfoLog/Concepts.h>

/// @brief 
namespace ExtendedCpp::InfoLog
{
    /// @brief 
    /// @tparam TSource 
    /// @param source 
    /// @return 
    template<Concepts::ConvertableToString TSource>
    std::string ToString(TSource&& source) noexcept
    {
        std::stringstream stream;
        stream << std::forward<TSource>(source);
        return stream.str();
    }

    /// @brief 
    /// @tparam T 
    /// @param source 
    /// @return 
    template<Concepts::ConvertableFromString T>
    T FromString(const std::string& source) noexcept
    {
        std::stringstream stream(source);
        T result;
        stream >> result;
        return result;
    }

    /// @brief 
    /// @tparam T 
    /// @param source 
    /// @return 
    template<Concepts::ConvertableFromString T>
    T FromString(std::string&& source) noexcept
    {
        std::stringstream stream(std::move(source));
        T result;
        stream >> result;
        return result;
    }

    /// @brief 
    /// @param string 
    /// @return 
    inline std::string ToLowerCase(const std::string& string) noexcept
    {
        std::string copy = string;
        std::ranges::transform(copy, copy.begin(),
            [](const unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return copy;
    }

    /// @brief 
    /// @param string 
    /// @return 
    inline std::string ToLowerCase(std::string&& string) noexcept
    {
        std::ranges::transform(string, string.begin(),
            [](const unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return string;
    }

    /// @brief 
    /// @param str 
    /// @param from 
    /// @param to 
    inline void ReplaceAll(std::string& str, const std::string& from, const std::string& to) noexcept
    {
        if (str.empty() || from.empty() || to.empty()) return;

        size_t startPosition = 0;
        while((startPosition = str.find(from, startPosition)) != std::string::npos)
        {
            str.replace(startPosition, from.length(), to);
            startPosition += to.length();
        }
    }
}

#endif