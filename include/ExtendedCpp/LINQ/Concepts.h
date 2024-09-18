#ifndef LINQ_Concepts_H
#define LINQ_Concepts_H

#include <utility>
#include <optional>

namespace ExtendedCpp::LINQ::Concepts
{
    template<typename TIter>
    concept OptionalIter = requires(TIter iter)
    {
        { *(++iter) } -> std::same_as<std::optional<typename TIter::value_type>>;
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

    template<typename T>
    concept Iterable = requires(T collection)
    {
        collection.begin();
        collection.end();
        typename T::iterator;
    };

    template<typename T>
    concept ConstIterable = requires(T collection)
    {
        collection.cbegin();
        collection.cend();
        typename T::const_iterator;
    };

    template<typename T>
    concept HasSize = requires(T collection)
    {
        collection.size();
        collection.empty();
    };

    template<typename T, typename... TArgs>
    concept IsPredicate = requires(T predicate, TArgs... args)
    {
        { predicate(std::forward<TArgs>(args)...) } -> std::convertible_to<bool>;
    };
}

#endif