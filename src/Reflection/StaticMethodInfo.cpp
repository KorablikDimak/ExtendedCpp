#include <ExtendedCpp/Reflection/StaticMethodInfo.h>

std::any ExtendedCpp::Reflection::StaticMethodInfo::Invoke(const std::span<const std::any> args) const
{
    return _methodWithAny(_methodHelper, args);
}

std::any ExtendedCpp::Reflection::StaticMethodInfo::Invoke(const std::span<const std::shared_ptr<void>> args) const
{
    return _methodWithPtr(_methodHelper, args);
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::StaticMethodInfo::GetMemberType() const noexcept
{
    return MemberType::StaticMethod;
}

std::vector<std::type_index> ExtendedCpp::Reflection::StaticMethodInfo::Parameters() const noexcept
{
    return _parameters;
}