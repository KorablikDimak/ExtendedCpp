#include <Reflection/StaticMethodInfo.h>

Reflection::MemberType Reflection::StaticMethodInfo::MemberType() const noexcept
{
    return MemberType::StaticMethod;
}

std::vector<std::type_index> Reflection::StaticMethodInfo::Parameters() const noexcept
{
    return _parameters;
}