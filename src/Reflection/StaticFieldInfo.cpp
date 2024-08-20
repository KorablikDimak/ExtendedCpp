#include <Reflection/StaticFieldInfo.h>

std::type_index Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

Reflection::MemberType Reflection::StaticFieldInfo::MemberType() const noexcept
{
    return MemberType::StaticField;
}