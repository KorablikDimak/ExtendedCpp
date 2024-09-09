#include <Reflection/StaticFieldInfo.h>

std::any Reflection::StaticFieldInfo::GetField() const noexcept
{
    return _fieldGetter(_fieldHelper);
}

std::type_index Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

Reflection::MemberType Reflection::StaticFieldInfo::MemberType() const noexcept
{
    return MemberType::StaticField;
}