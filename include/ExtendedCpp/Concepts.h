#ifndef Common_Concepts_H
#define Common_Concepts_H

#include <utility>

namespace ExtendedCpp::Concepts
{
    template<typename TFunctor, typename ...Args>
    concept IsFunctor = requires
    {
        std::declval<TFunctor>()(std::declval<Args>()...);
    };

    template<typename T>
    concept Equatable = requires(T value)
    {
        { value == value } -> std::convertible_to<bool>;
        { value != value } -> std::convertible_to<bool>;
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
    concept ConstructableFromNumber = requires
    {
        T(std::declval<int>());
    };

    template<typename T>
    concept Summarize = requires(T value)
    {
        { value + value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Substitute = requires(T value)
    {
        { value - value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Multiply = requires(T value)
    {
        { value * value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Divisible = requires(T value)
    {
        { value / value } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Negative = requires(T value)
    {
        { -value } -> std::convertible_to<T>;
    };
}

#endif