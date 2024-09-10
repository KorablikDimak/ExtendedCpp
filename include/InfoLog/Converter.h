#ifndef InfoLog_Converter_H
#define InfoLog_Converter_H

#include <algorithm>

#include <InfoLog/Concepts.h>

namespace InfoLog
{
    template<Concepts::ConvertableToString TSource>
    std::string ToString(TSource&& source) noexcept
    {
        std::stringstream stream;
        stream << std::forward<TSource>(source);
        return stream.str();
    }

    template<Concepts::ConvertableFromString T>
    T FromString(const std::string& source) noexcept
    {
        std::stringstream stream(source);
        T result;
        stream >> result;
        return std::move(result);
    }

    template<Concepts::ConvertableFromString T>
    T FromString(std::string&& source) noexcept
    {
        std::stringstream stream(std::move(source));
        T result;
        stream >> result;
        return std::move(result);
    }

    inline std::string ToLowerCase(const std::string& string) noexcept
    {
        std::string copy = string;
        std::ranges::transform(copy, copy.begin(),
            [](const unsigned char c){ return std::tolower(c); });
        return std::move(copy);
    }

    inline std::string ToLowerCase(std::string&& string) noexcept
    {
        std::ranges::transform(string, string.begin(),
            [](const unsigned char c){ return std::tolower(c); });
        return std::move(string);
    }

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