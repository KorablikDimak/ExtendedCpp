#ifndef LINQ_Concepts_H
#define LINQ_Concepts_H

#include <utility>

/// @brief 
namespace ExtendedCpp::LINQ::Concepts
{
    template<typename TPair>
    concept IsPair = requires(TPair pair)
    {
        pair.first;
        pair.second;
    };

    template<typename T>
    concept Divisible = requires(T value)
    {
        { value / std::declval<std::size_t>() } -> std::convertible_to<std::decay_t<T>>;
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
    concept Summarize = requires(T value)
    {
        { value + value } -> std::convertible_to<std::decay_t<T>>;
    };

    template<typename T>
    concept Equatable = requires(T value)
    {
        { value == value } -> std::convertible_to<bool>;
        { value != value } -> std::convertible_to<bool>;
    };

    template<typename TCollection>
    concept InputIterable = std::input_iterator<typename std::decay_t<TCollection>::iterator>;

    template<typename TCollection>
    concept ForwardIterable = std::forward_iterator<typename std::decay_t<TCollection>::iterator>;

    template<typename TCollection>
    concept RandomAccess = requires(TCollection collection)
    {
        collection[std::declval<std::size_t>()];
    };

    template<typename TCollection>
    concept HasSize = requires(TCollection collection)
    {
        collection.size();
        collection.empty();
    };

    template<typename TPredicate, typename... TArgs>
    concept IsPredicate = requires(TPredicate predicate, TArgs... args)
    {
        { predicate(std::forward<TArgs>(args)...) } -> std::convertible_to<bool>;
    };

    template<typename TCollection>
    concept LinqCollection = std::decay_t<TCollection>::IsLinqCollection;

    template<typename TStruct>
    concept LinqAdaptor = std::decay_t<TStruct>::IsLinqAdaptor;
}

#endif