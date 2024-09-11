module;

#include <optional>
#include <vector>
#include <typeindex>
#include <string>

export module ExtendedCpp.Reflection.Reflection;

import ExtendedCpp.Reflection.TypeInfo;
import ExtendedCpp.Reflection.Concepts;

export namespace ExtendedCpp::Reflection
{
    std::vector<TypeInfo> GetType(const std::string& typeName) noexcept
    {
        std::vector<TypeInfo> info;
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()).find(typeName) != std::string_view::npos)
                info.push_back(type);
        return std::move(info);
    }

    std::optional<TypeInfo> GetType(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type;
        return std::nullopt;
    }

    template<Concepts::HasMetaInfo T>
    TypeInfo GetType() noexcept
    {
        return T::MetaInfo;
    }
}