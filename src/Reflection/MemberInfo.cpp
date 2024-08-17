#include <Reflection/MemberInfo.h>

#include <utility>

Reflection::MemberInfo::MemberInfo(const std::string& name) noexcept
{
    _name = name;
}

Reflection::MemberInfo::MemberInfo(std::string&& name) noexcept
{
    _name = std::move(name);
}

std::string Reflection::MemberInfo::Name() const noexcept
{
    return _name;
}

Reflection::MemberType Reflection::MemberInfo::MemberType() const noexcept
{
    return MemberType::Unknown;
}
