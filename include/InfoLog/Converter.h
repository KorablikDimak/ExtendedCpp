#ifndef InfoLog_Converter_H
#define InfoLog_Converter_H

#include <algorithm>
#include <sstream>

namespace InfoLog
{
    template<typename TSource>
    std::string ToString(const TSource& source)
    {
        std::stringstream stream;
        stream << source;
        return stream.str();
    }

    template<typename T>
    T FromString(const std::string& source)
    {
        std::stringstream stream(source);
        T result;
        stream >> result;
        return result;
    }

    inline std::string ToLowerCase(const std::string& string)
    {
        std::string copy = string;
        std::ranges::transform(std::move(copy), copy.begin(),
            [](const unsigned char c){ return std::tolower(c); });
        return copy;
    }

    inline void ToLowerCase(std::string&& string)
    {
        std::ranges::transform(std::forward<std::string>(string), string.begin(),
            [](const unsigned char c){ return std::tolower(c); });
    }

    inline void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
    {
        size_t startPosition = 0;
        while((startPosition = str.find(from, startPosition)) != std::string::npos)
        {
            str.replace(startPosition, from.length(), to);
            startPosition += to.length();
        }
    }
}

#endif