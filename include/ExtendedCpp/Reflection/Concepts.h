#ifndef REFLECTION_CONCEPTS_H
#define REFLECTION_CONCEPTS_H

#include <any>
#include <memory>
#include <vector>

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
        std::same_as<std::decay_t<T>, std::vector<std::any>> ||
        std::same_as<std::decay_t<T>,std::vector<std::shared_ptr<void>>>;

    template<typename... TArgs>
    concept ValidConstructorArgs = sizeof...(TArgs) != 1 || (!IsArgumentCollection<TArgs> && ...);
}

#endif