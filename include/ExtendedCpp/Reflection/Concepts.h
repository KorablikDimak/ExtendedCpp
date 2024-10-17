#ifndef Reflection_Concepts_H
#define Reflection_Concepts_H

#include <memory>

namespace ExtendedCpp::Reflection::Concepts
{
    template<typename T>
    concept HasMetaInfo = requires
    {
        T::MetaInfo;
    };

    template<typename T>
    concept IsSharedPtr = std::same_as<T, std::shared_ptr<typename T::element_type>>;
}

#endif