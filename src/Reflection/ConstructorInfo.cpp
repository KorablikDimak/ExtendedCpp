#include <ExtendedCpp/Reflection/ConstructorInfo.h>

std::any ExtendedCpp::Reflection::ConstructorInfo::Create(const std::span<const std::any> args) const
{
    return _createFromAny(args);
}

std::any ExtendedCpp::Reflection::ConstructorInfo::Create(const std::span<const std::shared_ptr<void>> args) const
{
    return _createFromPtr(args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::New(const std::span<const std::any> args) const
{
    return _newFromAny(args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::New(const std::span<const std::shared_ptr<void>> args) const
{
    return _newFromPtr(args);
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::ConstructorInfo::GetMemberType() const noexcept
{
    return MemberType::Constructor;
}

std::vector<std::type_index> ExtendedCpp::Reflection::ConstructorInfo::Parameters() const noexcept
{
    return _parameters;
}