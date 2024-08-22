#ifndef Reflection_TypeInfo_H
#define Reflection_TypeInfo_H

#include <vector>
#include <memory>

#include <Reflection/FieldInfo.h>
#include <Reflection/MethodInfo.h>
#include <Reflection/ConstructorInfo.h>
#include <Reflection/StaticMethodInfo.h>
#include <Reflection/StaticFieldInfo.h>

namespace Reflection
{
    class TypeInfo final
    {
    private:
        std::type_index _typeIndex;
        std::vector<std::shared_ptr<MemberInfo>> _members;

    public:
        TypeInfo(std::type_index typeIndex, std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        ~TypeInfo() = default;

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

#define META_DECL \
static const Reflection::TypeInfo MetaInfo;

#define META_IMPL(className, ...) \
using ThisClassType = className; \
const Reflection::TypeInfo ThisClassType::MetaInfo = Reflection::TypeInfo(typeid(ThisClassType), {__VA_ARGS__});

#define META(className, ...) \
using ThisClassType = className; \
inline static const Reflection::TypeInfo MetaInfo = Reflection::TypeInfo(typeid(ThisClassType), {__VA_ARGS__});

#endif