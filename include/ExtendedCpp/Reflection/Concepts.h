#ifndef Reflection_Concepts_H
#define Reflection_Concepts_H

#include <memory>

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
}

#endif