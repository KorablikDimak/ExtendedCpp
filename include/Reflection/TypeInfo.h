#ifndef Reflection_TypeInfo_H
#define Reflection_TypeInfo_H

#include <vector>

#include <Reflection/FieldInfo.h>
#include <Reflection/MethodInfo.h>
#include <Reflection/ConstructorInfo.h>

namespace Reflection
{
    #define META(...) \
    TypeInfo MetaInfo = TypeInfo({__VA_ARGS__});

    class TypeInfo final
    {
    private:
        std::vector<std::shared_ptr<MemberInfo>> _members;

    public:
        TypeInfo(std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        ~TypeInfo() = default;

        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<FieldInfo>> GetFields() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods() const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors() const noexcept;

        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::shared_ptr<FieldInfo> GetField(const std::string_view& name) const noexcept;
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods(const std::string_view& name) const noexcept;
    };
}

#endif