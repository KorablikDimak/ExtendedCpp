#include <ExtendedCpp/Reflection/MethodInfo.h>

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::MethodInfo::MemberType() const noexcept
{
    return MemberType::Method;
}

std::vector<std::type_index> ExtendedCpp::Reflection::MethodInfo::Parameters() const noexcept
{
    return _parameters;
}

bool ExtendedCpp::Reflection::MethodInfo::HasConstOverload() const noexcept
{
    return _methodCvQualifier != MethodCVQualifier::OnlyNoConst;
}

bool ExtendedCpp::Reflection::MethodInfo::HasNoConstOverload() const noexcept
{
    return _methodCvQualifier != MethodCVQualifier::OnlyConst;
}