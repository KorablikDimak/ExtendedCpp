#ifndef InfoLog_Concepts_H
#define InfoLog_Concepts_H

#include <sstream>

namespace InfoLog::Concepts
{
    template<typename T>
    concept ConvertableToString = requires
    {
        std::declval<std::stringstream>() << std::declval<T>();
    };

    template<typename T>
    concept ConvertableFromString = requires
    {
        std::declval<std::stringstream>() >> std::declval<T>();
    };
}

#endif