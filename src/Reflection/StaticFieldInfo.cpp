#include <Reflection/StaticFieldInfo.h>

#include <utility>

Reflection::StaticFieldInfo::StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, void* field) noexcept
        : _typeIndex(typeIndex), _field(field), MemberInfo(fieldName) {}

Reflection::StaticFieldInfo::StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, void* field) noexcept
        : _typeIndex(typeIndex), _field(field), MemberInfo(std::move(fieldName)) {}

std::type_index Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

void* Reflection::StaticFieldInfo::Value() noexcept
{
    return _field;
}

Reflection::MemberType Reflection::StaticFieldInfo::MemberType() const noexcept
{
    return MemberType::StaticField;
}