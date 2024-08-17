#include <Reflection/TypeInfo.h>

Reflection::TypeInfo::TypeInfo(std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept
{
    _members = members;
}

std::vector<std::shared_ptr<Reflection::MemberInfo>>
Reflection::TypeInfo::GetMembers() const noexcept
{
    std::vector<std::shared_ptr<Reflection::MemberInfo>> result;
    result.reserve(_members.size());

    for (const auto& member : _members)
        result.push_back(member);

    return result;
}

std::vector<std::shared_ptr<Reflection::FieldInfo>>
Reflection::TypeInfo::GetFields() const noexcept
{
    std::vector<std::shared_ptr<Reflection::FieldInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Field)
            result.push_back(std::dynamic_pointer_cast<Reflection::FieldInfo>(member));

    return result;
}

std::vector<std::shared_ptr<Reflection::MethodInfo>>
Reflection::TypeInfo::GetMethods() const noexcept
{
    std::vector<std::shared_ptr<Reflection::MethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Method)
            result.push_back(std::dynamic_pointer_cast<Reflection::MethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<Reflection::ConstructorInfo>>
Reflection::TypeInfo::GetConstructors() const noexcept
{
    std::vector<std::shared_ptr<Reflection::ConstructorInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Constructor)
            result.push_back(std::dynamic_pointer_cast<Reflection::ConstructorInfo>(member));

    return result;
}

std::vector<std::shared_ptr<Reflection::MemberInfo>>
Reflection::TypeInfo::GetMembers(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<Reflection::MemberInfo>> result;
    result.reserve(_members.size());

    for (const auto& member : _members)
        if (member->Name() == name)
            result.push_back(member);

    return result;
}

std::shared_ptr<Reflection::FieldInfo>
Reflection::TypeInfo::GetField(const std::string_view& name) const noexcept
{
    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Field && member->Name() == name)
            return std::dynamic_pointer_cast<Reflection::FieldInfo>(member);

    return {nullptr};
}

std::vector<std::shared_ptr<Reflection::MethodInfo>>
Reflection::TypeInfo::GetMethods(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<Reflection::MethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Method && member->Name() == name)
            result.push_back(std::dynamic_pointer_cast<Reflection::MethodInfo>(member));

    return result;
}