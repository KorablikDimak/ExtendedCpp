#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <utility>

ExtendedCpp::Reflection::MemberInfo::MemberInfo(std::string&& name) noexcept
{
    _name = std::move(name);
}

std::string ExtendedCpp::Reflection::MemberInfo::Name() const noexcept
{
    return _name;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::MemberInfo::GetMemberType() const noexcept
{
    return MemberType::Unknown;
}
