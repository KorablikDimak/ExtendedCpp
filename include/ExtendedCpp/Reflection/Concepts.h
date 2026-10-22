#ifndef REFLECTION_CONCEPTS_H
#define REFLECTION_CONCEPTS_H

#include <any>
#include <memory>
#include <span>

/// @brief
namespace ExtendedCpp::Reflection::Concepts
{
    template<typename T>
    concept HasMetaInfo = requires
    {
        T::MetaInfo;
    };

    template<typename T>
    concept IsSharedPtr = std::same_as<std::decay_t<T>, std::shared_ptr<typename std::decay_t<T>::element_type>>;

    template<typename T>
    concept IsArgumentCollection =
        std::convertible_to<std::decay_t<T>, std::span<const std::any>> ||
        std::convertible_to<std::decay_t<T>, std::span<const std::shared_ptr<void>>>;

    template<typename... TArgs>
    concept ValidArgs = sizeof...(TArgs) != 1 || (!IsArgumentCollection<TArgs> && ...);
}

#endif