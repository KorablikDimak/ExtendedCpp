#include <ExtendedCpp/Reflection/StaticMethodInfo.h>

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::StaticMethodInfo::MemberType() const noexcept
{
    return MemberType::StaticMethod;
}

std::vector<std::type_index> ExtendedCpp::Reflection::StaticMethodInfo::Parameters() const noexcept
{
    return _parameters;
}