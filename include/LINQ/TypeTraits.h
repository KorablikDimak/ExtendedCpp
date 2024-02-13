#ifndef LINQ_TypeTraits_H
#define LINQ_TypeTraits_H

#include <utility>

namespace LINQ
{
    template<typename TFunctor, typename ...Args>
    concept IsFunctor = requires
    {
        std::declval<TFunctor>()(std::declval<Args>()...);
    };

    template<typename TFunctor>
    struct FunctorTraits;

    template<typename TFunctor, typename... Args>
    struct FunctorTraits<TFunctor(Args...)>
    {
        using ReturnType = decltype(std::declval<TFunctor>()(std::declval<Args>()...));
    };

    template<typename TPair>
    concept IsPair = requires(TPair pair)
    {
        pair.first;
        pair.second;
    };

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

    template<typename T>
    concept Divisible = requires(T value)
    {
        { value / std::declval<std::size_t>() } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Comparable = requires(T value)
    {
        { value < value } -> std::convertible_to<bool>;
        { value > value } -> std::convertible_to<bool>;
        { value <= value } -> std::convertible_to<bool>;
        { value >= value } -> std::convertible_to<bool>;
        { value == value } -> std::convertible_to<bool>;
        { value != value } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept Summarizable = requires(T value)
    {
        { value + value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Equalable = requires(T value)
    {
        { value == value } -> std::convertible_to<bool>;
        { value != value } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept Iterable = requires(T collection)
    {
        collection.begin();
        collection.end();
    };

    template<typename T>
    concept ConstIterable = requires(T collection)
    {
        collection.cbegin();
        collection.cend();
    };

    template<typename T>
    concept HasSize = requires(T collection)
    {
        collection.size();
        collection.empty();
    };
}

#endif