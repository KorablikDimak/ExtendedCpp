#include <Reflection/FieldInfo.h>

#include <utility>

Reflection::FieldInfo::FieldInfo(const std::string& fieldName, std::type_index typeIndex, void* field) noexcept
    : _typeIndex(typeIndex), _field(field), MemberInfo(fieldName) {}

Reflection::FieldInfo::FieldInfo(std::string&& fieldName, std::type_index typeIndex, void* field) noexcept
    : _typeIndex(typeIndex), _field(field), MemberInfo(std::move(fieldName)) {}

std::type_index Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

void* Reflection::FieldInfo::Value() noexcept
{
    return _field;
}

Reflection::MemberType Reflection::FieldInfo::MemberType() const noexcept
{
    return MemberType::Field;
}