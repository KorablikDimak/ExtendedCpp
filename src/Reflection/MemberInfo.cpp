#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <utility>

ExtendedCpp::Reflection::MemberInfo::MemberInfo(const std::string& name) noexcept
{
    _name = name;
}

ExtendedCpp::Reflection::MemberInfo::MemberInfo(std::string&& name) noexcept
{
    _name = std::move(name);
}

std::string ExtendedCpp::Reflection::MemberInfo::Name() const noexcept
{
    return _name;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::MemberInfo::MemberType() const noexcept
{
    return MemberType::Unknown;
}
