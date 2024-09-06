#include <Reflection/Reflection.h>

std::vector<Reflection::TypeInfo> Reflection::GetType(const std::string& typeName) noexcept
{
    std::vector<TypeInfo> info;
    for (const auto& type : Assembly::GetTypes())
        if (std::string_view(type.TypeIndex().name()).find(typeName) != std::string_view::npos)
            info.push_back(type);
    return info;
}

std::optional<Reflection::TypeInfo> Reflection::GetType(std::type_index typeIndex) noexcept
{
    for (const auto& type : Assembly::GetTypes())
        if (type.TypeIndex() == typeIndex)
            return type;
    return std::nullopt;
}