#ifndef LINQ_TypeTraits_H
#define LINQ_TypeTraits_H

#include <utility>

#include <ExtendedCpp/LINQ/Concepts.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief 
    /// @tparam TPair 
    /// @tparam  
    /// @tparam  
    template<typename TPair, typename = void, typename = void>
    struct PairTraits
    {
        using FirstType = void;
        using SecondType = void;
    };

    /// @brief 
    /// @tparam TPair 
    template<typename TPair>
    struct PairTraits<TPair, std::void_t<decltype(std::declval<TPair>().first)>,
                             std::void_t<decltype(std::declval<TPair>().second)>>
    {
        using FirstType = std::decay_t<decltype(std::declval<TPair>().first)>;
        using SecondType = std::decay_t<decltype(std::declval<TPair>().second)>;
    };

    /// @brief
    /// @tparam TCollection 
    template<Concepts::RandomAccess TCollection>
    struct RandomAccessValue
    {
        using Type = std::decay_t<decltype(std::declval<TCollection>()[std::declval<std::size_t>()])>;
    };

    /// @brief 
    /// @tparam TCollection 
    template <Concepts::RandomAccess TCollection>
    using RandomAccessValueType = typename RandomAccessValue<TCollection>::Type;
}

#endif