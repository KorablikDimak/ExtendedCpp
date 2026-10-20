#include <ExtendedCpp/Reflection/ConstructorInfo.h>

std::any ExtendedCpp::Reflection::ConstructorInfo::Create(const std::vector<std::any>& args) const
{
    return _fromAny(args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::New(const std::vector<std::any>& args) const
{
    return _fromAnyNew(args);
}

std::any ExtendedCpp::Reflection::ConstructorInfo::Create(const std::vector<std::shared_ptr<void>>& args) const
{
    return _fromPtr(args);
}

std::shared_ptr<void> ExtendedCpp::Reflection::ConstructorInfo::New(const std::vector<std::shared_ptr<void>>& args) const
{
    return _fromPtrNew(args);
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::ConstructorInfo::GetMemberType() const noexcept
{
    return MemberType::Constructor;
}

std::vector<std::type_index> ExtendedCpp::Reflection::ConstructorInfo::Parameters() const noexcept
{
    return _parameters;
}