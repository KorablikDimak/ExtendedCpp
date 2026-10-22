#include <utility>

#include <ExtendedCpp/Reflection/MemberInfo.h>

ExtendedCpp::Reflection::MemberInfo::MemberInfo(std::string&& name) noexcept : _name(std::move(name)) {}

std::string_view ExtendedCpp::Reflection::MemberInfo::Name() const noexcept
{
    return _name;
}