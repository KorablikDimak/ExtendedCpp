#include <ExtendedCpp/Reflection/Assembly.h>

std::map<std::type_index, ExtendedCpp::Reflection::TypeInfo>& ExtendedCpp::Reflection::Assembly::TypeList() noexcept
{
    static std::map<std::type_index, TypeInfo> typeList;
    return typeList;
}

std::mutex& ExtendedCpp::Reflection::Assembly::ListMutex() noexcept
{
    static std::mutex listMutex;
    return listMutex;
}

void ExtendedCpp::Reflection::Assembly::AddType(const TypeInfo& typeInfo) noexcept
{
    std::lock_guard<std::mutex> guard(ListMutex());
    TypeList().insert(std::make_pair(typeInfo.TypeIndex(), typeInfo));
}

std::vector<ExtendedCpp::Reflection::TypeInfo> ExtendedCpp::Reflection::Assembly::GetTypes() noexcept
{
    std::lock_guard<std::mutex> guard(ListMutex());
    std::vector<TypeInfo> types;
    types.reserve(TypeList().size());
    for (const auto& type : TypeList())
        types.push_back(type.second);
    return types;
}