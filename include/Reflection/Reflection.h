#ifndef Reflection_Reflection_H
#define Reflection_Reflection_H

#include <optional>

#include <Reflection/Assembly.h>

namespace Reflection
{
    inline std::vector<TypeInfo> GetType(const std::string& typeName) noexcept
    {
        std::vector<TypeInfo> info;
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()).find(typeName) != std::string_view::npos)
                info.push_back(type);
        return info;
    }

    template<typename T>
    std::optional<TypeInfo> GetType(T targetType) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(targetType))
                return type;
        return std::nullopt;
    }

    template<typename T>
    TypeInfo GetType() noexcept
    {
        return T::MetaInfo;
    }
}

#endif