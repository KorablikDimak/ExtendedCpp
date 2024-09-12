#include <ExtendedCpp/Reflection.h>

std::vector<ExtendedCpp::Reflection::TypeInfo> ExtendedCpp::Reflection::GetType(const std::string& typeName) noexcept
{
    std::vector<TypeInfo> info;
    for (const auto& type : Assembly::GetTypes())
        if (std::string_view(type.TypeIndex().name()).find(typeName) != std::string_view::npos)
            info.push_back(type);
    return std::move(info);
}

std::optional<ExtendedCpp::Reflection::TypeInfo> ExtendedCpp::Reflection::GetType(std::type_index typeIndex) noexcept
{
    for (const auto& type : Assembly::GetTypes())
        if (type.TypeIndex() == typeIndex)
            return type;
    return std::nullopt;
}