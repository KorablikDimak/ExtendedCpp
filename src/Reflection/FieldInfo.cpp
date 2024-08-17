#include <Reflection/FieldInfo.h>

#include <utility>

Reflection::FieldInfo::FieldInfo(const std::string& fieldName, void* field) noexcept
    : MemberInfo(fieldName)
{
    _field = field;
}

Reflection::FieldInfo::FieldInfo(std::string&& fieldName, void* field) noexcept
    : MemberInfo(std::move(fieldName))
{
    _field = field;
}

void* Reflection::FieldInfo::Value() noexcept
{
    return _field;
}

Reflection::MemberType Reflection::FieldInfo::MemberType() const noexcept
{
    return MemberType::Field;
}
