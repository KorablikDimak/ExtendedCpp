module;

#include <vector>
#include <memory>
#include <string>
#include <typeindex>

export module ExtendedCpp.Reflection.TypeInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.ConstructorInfo;
import ExtendedCpp.Reflection.Assembly;
import ExtendedCpp.Reflection.FieldInfo;
import ExtendedCpp.Reflection.StaticFieldInfo;
import ExtendedCpp.Reflection.MethodInfo;
import ExtendedCpp.Reflection.StaticMethodInfo;

export namespace ExtendedCpp::Reflection
{
    class TypeInfo final
    {
    private:
        std::string _name;
        std::type_index _typeIndex;
        std::vector<std::shared_ptr<MemberInfo>> _members{};

    public:
        TypeInfo(const std::string& name, std::type_index typeIndex,
                 std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept :
            _name(name), _typeIndex(typeIndex), _members(members)
        {
            Assembly::AddType(*this);
        }

        TypeInfo(std::string&& name, std::type_index typeIndex,
                 std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept :
            _name(std::move(name)), _typeIndex(typeIndex), _members(members)
        {
            Assembly::AddType(*this);
        }

        ~TypeInfo() = default;

        [[nodiscard]]
        std::string Name() const noexcept
        {
            return _name;
        }

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept
        {
            return _typeIndex;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::MemberInfo>> result;
            result.reserve(_members.size());

            for (const auto& member : _members)
                result.push_back(member);

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<FieldInfo>> GetFields() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::FieldInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::Field)
                    result.push_back(std::dynamic_pointer_cast<Reflection::FieldInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<StaticFieldInfo>> GetStaticFields() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::StaticFieldInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::StaticField)
                    result.push_back(std::dynamic_pointer_cast<Reflection::StaticFieldInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::MethodInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::Method)
                    result.push_back(std::dynamic_pointer_cast<Reflection::MethodInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::StaticMethodInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::StaticMethod)
                    result.push_back(std::dynamic_pointer_cast<Reflection::StaticMethodInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors() const noexcept
        {
            std::vector<std::shared_ptr<Reflection::ConstructorInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::Constructor)
                    result.push_back(std::dynamic_pointer_cast<Reflection::ConstructorInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers(const std::string_view& name) const noexcept
        {
            std::vector<std::shared_ptr<Reflection::MemberInfo>> result;
            result.reserve(_members.size());

            for (const auto& member : _members)
                if (member->Name() == name)
                    result.push_back(member);

            return result;
        }

        [[nodiscard]]
        std::shared_ptr<FieldInfo> GetField(const std::string_view& name) const noexcept
        {
            for (const auto& member : _members)
                if (member->MemberType() == MemberType::Field && member->Name() == name)
                    return std::dynamic_pointer_cast<Reflection::FieldInfo>(member);

            return {nullptr};
        }

        [[nodiscard]]
        std::shared_ptr<StaticFieldInfo> GetStaticField(const std::string_view& name) const noexcept
        {
            for (const auto& member : _members)
                if (member->MemberType() == MemberType::StaticField && member->Name() == name)
                    return std::dynamic_pointer_cast<Reflection::StaticFieldInfo>(member);

            return {nullptr};
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods(const std::string_view& name) const noexcept
        {
            std::vector<std::shared_ptr<Reflection::MethodInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::Method && member->Name() == name)
                    result.push_back(std::dynamic_pointer_cast<Reflection::MethodInfo>(member));

            return result;
        }

        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods(const std::string_view& name) const noexcept
        {
            std::vector<std::shared_ptr<Reflection::StaticMethodInfo>> result;

            for (const auto& member : _members)
                if (member->MemberType() == MemberType::StaticMethod && member->Name() == name)
                    result.push_back(std::dynamic_pointer_cast<Reflection::StaticMethodInfo>(member));

            return result;
        }
    };
}