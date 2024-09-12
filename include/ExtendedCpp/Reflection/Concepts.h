#ifndef Reflection_Concepts_H
#define Reflection_Concepts_H

namespace ExtendedCpp::Reflection::Concepts
{
    template<typename T>
    concept HasMetaInfo = requires
    {
        T::MetaInfo;
    };
}

#endif