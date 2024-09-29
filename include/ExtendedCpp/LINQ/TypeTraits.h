#ifndef LINQ_TypeTraits_H
#define LINQ_TypeTraits_H

#include <utility>

namespace ExtendedCpp::LINQ
{
    template<typename TPair, typename = void, typename = void>
    struct PairTraits
    {
        using FirstType = void;
        using SecondType = void;
    };

    template<typename TPair>
    struct PairTraits<TPair, std::void_t<decltype(std::declval<TPair>().first)>, std::void_t<decltype(std::declval<TPair>().second)>>
    {
        using FirstType = decltype(std::declval<TPair>().first);
        using SecondType = decltype(std::declval<TPair>().second);
    };
}

#endif