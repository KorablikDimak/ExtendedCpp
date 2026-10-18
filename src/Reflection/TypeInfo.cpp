#include <ExtendedCpp/Reflection/Assembly.h>
#include <ExtendedCpp/Reflection/TypeInfo.h>

ExtendedCpp::Reflection::TypeInfo::TypeInfo(const std::string_view name, const std::type_index typeIndex,
                                            const std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept :
    _name(name), _typeIndex(typeIndex), _members(members)
{
    Assembly::AddType(*this);
}

std::string ExtendedCpp::Reflection::TypeInfo::Name() const noexcept
{
    return _name;
}

std::type_index ExtendedCpp::Reflection::TypeInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

const std::vector<std::shared_ptr<ExtendedCpp::Reflection::MemberInfo>>&
ExtendedCpp::Reflection::TypeInfo::GetMembers() const noexcept
{
    return _members;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::FieldInfo>>
ExtendedCpp::Reflection::TypeInfo::GetFields() const noexcept
{
    std::vector<std::shared_ptr<FieldInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Field)
            result.emplace_back(std::static_pointer_cast<FieldInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticFieldInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticFields() const noexcept
{
    std::vector<std::shared_ptr<StaticFieldInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::StaticField)
            result.emplace_back(std::static_pointer_cast<StaticFieldInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMethods() const noexcept
{
    std::vector<std::shared_ptr<MethodInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Method)
            result.emplace_back(std::static_pointer_cast<MethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticMethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticMethods() const noexcept
{
    std::vector<std::shared_ptr<StaticMethodInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::StaticMethod)
            result.emplace_back(std::static_pointer_cast<StaticMethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::ConstructorInfo>>
ExtendedCpp::Reflection::TypeInfo::GetConstructors() const noexcept
{
    std::vector<std::shared_ptr<ConstructorInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Constructor)
            result.emplace_back(std::static_pointer_cast<ConstructorInfo>(member));

    return result;
}

std::shared_ptr<ExtendedCpp::Reflection::ConstructorInfo>
ExtendedCpp::Reflection::TypeInfo::GetDefaultConstructor() const noexcept
{
    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Constructor)
        {
            const auto constructor = std::static_pointer_cast<ConstructorInfo>(member);
            if (constructor->Parameters().empty())
                return constructor;
        }

    return { nullptr };
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MemberInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMembers(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<MemberInfo>> result;
    result.reserve(_members.size());

    for (const auto& member : _members)
        if (member->Name() == name)
            result.emplace_back(member);

    return result;
}

std::shared_ptr<ExtendedCpp::Reflection::FieldInfo>
ExtendedCpp::Reflection::TypeInfo::GetField(const std::string_view& name) const noexcept
{
    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Field && member->Name() == name)
            return std::static_pointer_cast<FieldInfo>(member);

    return { nullptr };
}

std::shared_ptr<ExtendedCpp::Reflection::StaticFieldInfo>
ExtendedCpp::Reflection::TypeInfo::GetStaticField(const std::string_view& name) const noexcept
{
    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::StaticField && member->Name() == name)
            return std::static_pointer_cast<StaticFieldInfo>(member);

    return { nullptr };
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMethods(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<MethodInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::Method && member->Name() == name)
            result.emplace_back(std::static_pointer_cast<MethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticMethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticMethods(const std::string_view &name) const noexcept
{
    std::vector<std::shared_ptr<StaticMethodInfo>> result;

    for (const auto& member : _members)
        if (member->GetMemberType() == MemberType::StaticMethod && member->Name() == name)
            result.emplace_back(std::static_pointer_cast<StaticMethodInfo>(member));

    return result;
}