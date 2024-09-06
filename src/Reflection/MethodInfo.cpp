#include <Reflection/MethodInfo.h>

Reflection::MemberType Reflection::MethodInfo::MemberType() const noexcept
{
    return MemberType::Method;
}

std::vector<std::type_index> Reflection::MethodInfo::Parameters() const noexcept
{
    return _parameters;
}