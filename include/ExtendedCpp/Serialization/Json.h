#ifndef SERIALIZATION_JSON_H
#define SERIALIZATION_JSON_H

#include <ExtendedCpp/Json.h>
#include <ExtendedCpp/Reflection.h>

namespace ExtendedCpp::Serialization::Json
{
    template<Reflection::Concepts::HasMetaInfo TTarget>
    TTarget To(const ExtendedCpp::Json&)
    {
        std::terminate();
    }

    template<Reflection::Concepts::HasMetaInfo TObject>
    ExtendedCpp::Json From(const TObject&)
    {
        std::terminate();
    }

    void foo();
}

#endif