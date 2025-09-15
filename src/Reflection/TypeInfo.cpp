#include <ExtendedCpp/Reflection/TypeInfo.h>
#include <ExtendedCpp/Reflection/Assembly.h>

ExtendedCpp::Reflection::TypeInfo::TypeInfo(const std::string& name, const std::type_index typeIndex,
                                            const std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept :
    _name(name), _typeIndex(typeIndex), _members(members)
{
    Assembly::AddType(*this);
}

ExtendedCpp::Reflection::TypeInfo::TypeInfo(std::string&& name, const std::type_index typeIndex,
                                            const std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept :
    _name(std::move(name)), _typeIndex(typeIndex), _members(members)
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

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MemberInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMembers() const noexcept
{
    std::vector<std::shared_ptr<MemberInfo>> result;
    result.reserve(_members.size());

    for (const auto& member : _members)
        result.push_back(member);

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::FieldInfo>>
ExtendedCpp::Reflection::TypeInfo::GetFields() const noexcept
{
    std::vector<std::shared_ptr<FieldInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Field)
            result.push_back(std::dynamic_pointer_cast<FieldInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticFieldInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticFields() const noexcept
{
    std::vector<std::shared_ptr<StaticFieldInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::StaticField)
            result.push_back(std::dynamic_pointer_cast<StaticFieldInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMethods() const noexcept
{
    std::vector<std::shared_ptr<MethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Method)
            result.push_back(std::dynamic_pointer_cast<MethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticMethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticMethods() const noexcept
{
    std::vector<std::shared_ptr<StaticMethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::StaticMethod)
            result.push_back(std::dynamic_pointer_cast<StaticMethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::ConstructorInfo>>
ExtendedCpp::Reflection::TypeInfo::GetConstructors() const noexcept
{
    std::vector<std::shared_ptr<ConstructorInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Constructor)
            result.push_back(std::dynamic_pointer_cast<ConstructorInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MemberInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMembers(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<MemberInfo>> result;
    result.reserve(_members.size());

    for (const auto& member : _members)
        if (member->Name() == name)
            result.push_back(member);

    return result;
}

std::shared_ptr<ExtendedCpp::Reflection::FieldInfo>
ExtendedCpp::Reflection::TypeInfo::GetField(const std::string_view& name) const noexcept
{
    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Field && member->Name() == name)
            return std::dynamic_pointer_cast<FieldInfo>(member);

    return {nullptr};
}

std::shared_ptr<ExtendedCpp::Reflection::StaticFieldInfo>
ExtendedCpp::Reflection::TypeInfo::GetStaticField(const std::string_view& name) const noexcept
{
    for (const auto& member : _members)
        if (member->MemberType() == MemberType::StaticField && member->Name() == name)
            return std::dynamic_pointer_cast<StaticFieldInfo>(member);

    return {nullptr};
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::MethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetMethods(const std::string_view& name) const noexcept
{
    std::vector<std::shared_ptr<MethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::Method && member->Name() == name)
            result.push_back(std::dynamic_pointer_cast<MethodInfo>(member));

    return result;
}

std::vector<std::shared_ptr<ExtendedCpp::Reflection::StaticMethodInfo>>
ExtendedCpp::Reflection::TypeInfo::GetStaticMethods(const std::string_view &name) const noexcept
{
    std::vector<std::shared_ptr<StaticMethodInfo>> result;

    for (const auto& member : _members)
        if (member->MemberType() == MemberType::StaticMethod && member->Name() == name)
            result.push_back(std::dynamic_pointer_cast<StaticMethodInfo>(member));

    return result;
}