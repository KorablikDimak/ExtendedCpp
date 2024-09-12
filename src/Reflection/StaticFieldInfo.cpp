#include <ExtendedCpp/Reflection/StaticFieldInfo.h>

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetField() const noexcept
{
    return _fieldGetter(_fieldHelper);
}

std::type_index ExtendedCpp::Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::StaticFieldInfo::MemberType() const noexcept
{
    return MemberType::StaticField;
}