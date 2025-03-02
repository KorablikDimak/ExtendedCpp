#ifndef LINQ_Concepts_H
#define LINQ_Concepts_H

#include <utility>
#include <optional>
#include <coroutine>

/// @brief 
namespace ExtendedCpp::LINQ::Concepts
{
    template<typename TIter>
    concept OptionalIter = requires(TIter iter)
    {
        { *(++iter) } -> std::same_as<std::optional<typename TIter::value_type>>;
        { iter == iter } -> std::convertible_to<bool>;
        { iter != iter } -> std::convertible_to<bool>;
    };

    template<typename TPair>
    concept IsPair = requires(TPair pair)
    {
        pair.first;
        pair.second;
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
    concept Summarize = requires(T value)
    {
        { value + value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Equatable = requires(T value)
    {
        { value == value } -> std::convertible_to<bool>;
        { value != value } -> std::convertible_to<bool>;
    };

    template<typename TCollection>
    concept Iterable = requires(TCollection collection)
    {
        collection.begin();
        collection.end();
        typename TCollection::iterator;
        typename TCollection::value_type;
    };

    template<typename TCollection>
    concept ConstIterable = requires(TCollection collection)
    {
        collection.cbegin();
        collection.cend();
        typename TCollection::const_iterator;
        typename TCollection::value_type;
    };

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
}

#endif