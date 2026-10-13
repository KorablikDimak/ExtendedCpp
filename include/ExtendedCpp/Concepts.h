#ifndef COMMON_CONCEPTS_H
#define COMMON_CONCEPTS_H

#include <concepts>

/// @brief
namespace ExtendedCpp::Concepts
{
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