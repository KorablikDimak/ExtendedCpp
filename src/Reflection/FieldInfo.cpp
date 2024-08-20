#include <Reflection/FieldInfo.h>

std::type_index Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

Reflection::MemberType Reflection::FieldInfo::MemberType() const noexcept
{
    return MemberType::Field;
}