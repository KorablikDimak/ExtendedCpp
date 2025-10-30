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
    concept Addable = requires(T value)
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

    template<typename TAdaptor, typename TLinqCollection>
    concept LinqAdaptor =
        std::is_invocable_v<TAdaptor, TLinqCollection> &&
        LinqCollection<TLinqCollection>;

    template<typename TRange>
    concept Range = requires(TRange range)
    {
        range.begin();
        range.end();
    };

    template<typename TLeft, typename TRight = TLeft>
    concept IsNothrowAddable = noexcept(std::declval<TLeft>() + std::declval<TRight>());

    template<typename TLeft, typename TRight = TLeft>
    concept IsNothrowDivisible = noexcept(std::declval<TLeft>() / std::declval<TRight>());

    template<typename TCollection>
    concept IsNothrowIterable =
        noexcept(++std::declval<std::decay_t<TCollection>>().begin()) ||
        std::is_pointer_v<typename std::decay_t<TCollection>::iterator>;
}

#endif