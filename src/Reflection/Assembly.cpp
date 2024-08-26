#include <Reflection/Assembly.h>

std::map<std::type_index, Reflection::TypeInfo>  Reflection::Assembly::_typeList;
std::mutex Reflection::Assembly::_listMutex;

void Reflection::Assembly::AddType(const TypeInfo& typeInfo) noexcept
{
    std::lock_guard<std::mutex> guard(_listMutex);
    _typeList.insert(std::make_pair(typeInfo.TypeIndex(), typeInfo));
}

std::vector<Reflection::TypeInfo> Reflection::Assembly::GetTypes() noexcept
{
    std::lock_guard<std::mutex> guard(_listMutex);
    std::vector<TypeInfo> types;
    types.reserve(_typeList.size());
    for (const auto& type : _typeList)
        types.push_back(type.second);
    return types;
}