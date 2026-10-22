#include <ExtendedCpp/Reflection/MethodInfo.h>

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(std::any object, const std::span<const std::any> args) const
{
    return _methodWithAny(_methodHelper, std::move(object), args);
}

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(std::shared_ptr<void>& object, const std::span<const std::any> args) const
{
    if (!object)
        throw std::invalid_argument("Object is null.");

    return _methodFromPtrWithAny(_methodHelper, object, args);
}

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(const std::shared_ptr<void>& object, const std::span<const std::any> args) const
{
    if (!object)
        throw std::invalid_argument("Object is null.");

    return _constMethodFromPtrWithAny(_methodHelper, object, args);
}

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(std::any object, const std::span<const std::shared_ptr<void>> args) const
{
    return _methodWithPtr(_methodHelper, std::move(object), args);
}

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(std::shared_ptr<void>& object, const std::span<const std::shared_ptr<void>> args) const
{
    if (!object)
        throw std::invalid_argument("Object is null.");

    return _methodFromPtrWithPtr(_methodHelper, object, args);
}

std::any ExtendedCpp::Reflection::MethodInfo::Invoke(const std::shared_ptr<void>& object, const std::span<const std::shared_ptr<void>> args) const
{
    if (!object)
        throw std::invalid_argument("Object is null.");

    return _constMethodFromPtrWithPtr(_methodHelper, object, args);
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::MethodInfo::GetMemberType() const noexcept
{
    return MemberType::Method;
}

std::span<const std::type_index> ExtendedCpp::Reflection::MethodInfo::Parameters() const noexcept
{
    return _parameters;
}

bool ExtendedCpp::Reflection::MethodInfo::HasConstOverload() const noexcept
{
    return _methodCvQualifier != MethodCvQualifier::OnlyNoConst;
}

bool ExtendedCpp::Reflection::MethodInfo::HasNoConstOverload() const noexcept
{
    return _methodCvQualifier != MethodCvQualifier::OnlyConst;
}