#include <ExtendedCpp/Reflection/ConstructorInfo.h>

std::any ExtendedCpp::Reflection::ConstructorInfo::CreateFromAny(const std::vector<std::any>& args) const
{
    return _fromAny(_constructorHelper, args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::NewFromAny(const std::vector<std::any>& args) const
{
    return _fromAnyNew(_constructorHelper, args);
}

std::any ExtendedCpp::Reflection::ConstructorInfo::CreateFromAny(const std::vector<std::shared_ptr<void>>& args) const
{
    return _fromAnyPtr(_constructorHelper, args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::NewFromAny(const std::vector<std::shared_ptr<void>>& args) const
{
    return _fromAnyPtrNew(_constructorHelper, args);
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::ConstructorInfo::MemberType() const noexcept
{
    return MemberType::Constructor;
}

std::vector<std::type_index> ExtendedCpp::Reflection::ConstructorInfo::Parameters() const noexcept
{
    return _parameters;
}