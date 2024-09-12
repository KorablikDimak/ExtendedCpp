#include <ExtendedCpp/Reflection/FieldInfo.h>

std::type_index ExtendedCpp::Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::FieldInfo::MemberType() const noexcept
{
    return MemberType::Field;
}