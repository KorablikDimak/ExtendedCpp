#ifndef Reflection_Reflection_H
#define Reflection_Reflection_H

#include <optional>

#include <Reflection/Assembly.h>
#include <Reflection/Concepts.h>

namespace Reflection
{
    std::vector<TypeInfo> GetType(const std::string& typeName) noexcept;
    std::optional<TypeInfo> GetType(std::type_index typeIndex) noexcept;

    template<Concepts::HasMetaInfo T>
    TypeInfo GetType() noexcept
    {
        return T::MetaInfo;
    }
}

#endif