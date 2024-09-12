#ifndef Reflection_TypeInfo_H
#define Reflection_TypeInfo_H

#include <vector>
#include <memory>

#include <ExtendedCpp/Reflection/FieldInfo.h>
#include <ExtendedCpp/Reflection/MethodInfo.h>
#include <ExtendedCpp/Reflection/ConstructorInfo.h>
#include <ExtendedCpp/Reflection/StaticMethodInfo.h>
#include <ExtendedCpp/Reflection/StaticFieldInfo.h>

namespace ExtendedCpp::Reflection
{
    class TypeInfo final
    {
    private:
        std::string _name;
        std::type_index _typeIndex;
        std::vector<std::shared_ptr<MemberInfo>> _members;

    public:
        TypeInfo(const std::string& name, std::type_index typeIndex, std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        TypeInfo(std::string&& name, std::type_index typeIndex, std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        ~TypeInfo() = default;

        [[nodiscard]]
        std::string Name() const noexcept;
        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<FieldInfo>> GetFields() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticFieldInfo>> GetStaticFields() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors() const noexcept;

        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::shared_ptr<FieldInfo> GetField(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::shared_ptr<StaticFieldInfo> GetStaticField(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods(const std::string_view& name) const noexcept;
    };
}

#define META(className, ...) \
using ThisClassType = className; \
static ExtendedCpp::Reflection::TypeInfo GetMetaInfo() \
{ \
    return ExtendedCpp::Reflection::TypeInfo(#className, typeid(ThisClassType), {__VA_ARGS__}); \
} \
inline static const ExtendedCpp::Reflection::TypeInfo MetaInfo = GetMetaInfo();

#endif