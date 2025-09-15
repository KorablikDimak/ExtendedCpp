#ifndef Reflection_TypeInfo_H
#define Reflection_TypeInfo_H

#include <vector>
#include <memory>

#include <ExtendedCpp/Reflection/CollectionFieldInfo.h>
#include <ExtendedCpp/Reflection/MethodInfo.h>
#include <ExtendedCpp/Reflection/ConstructorInfo.h>
#include <ExtendedCpp/Reflection/StaticMethodInfo.h>
#include <ExtendedCpp/Reflection/StaticFieldInfo.h>

/// @brief 
namespace ExtendedCpp::Reflection
{
    /// @brief 
    class TypeInfo final
    {
    private:
        std::string _name;
        std::type_index _typeIndex;
        std::vector<std::shared_ptr<MemberInfo>> _members;

    public:
        /// @brief 
        /// @param name 
        /// @param typeIndex 
        /// @param members 
        TypeInfo(const std::string& name, std::type_index typeIndex, std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        
        /// @brief 
        /// @param name 
        /// @param typeIndex 
        /// @param members 
        TypeInfo(std::string&& name, std::type_index typeIndex, std::initializer_list<std::shared_ptr<MemberInfo>> members) noexcept;
        
        /// @brief 
        ~TypeInfo() = default;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::string Name() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<FieldInfo>> GetFields() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticFieldInfo>> GetStaticFields() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors() const noexcept;

        /// @brief 
        /// @param name 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<MemberInfo>> GetMembers(const std::string_view& name) const noexcept;

        /// @brief 
        /// @param name 
        /// @return 
        [[nodiscard]]
        std::shared_ptr<FieldInfo> GetField(const std::string_view& name) const noexcept;

        /// @brief 
        /// @param name 
        /// @return 
        [[nodiscard]]
        std::shared_ptr<StaticFieldInfo> GetStaticField(const std::string_view& name) const noexcept;

        /// @brief 
        /// @param name 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<MethodInfo>> GetMethods(const std::string_view& name) const noexcept;

        /// @brief 
        /// @param name 
        /// @return 
        [[nodiscard]]
        std::vector<std::shared_ptr<StaticMethodInfo>> GetStaticMethods(const std::string_view& name) const noexcept;
    };
}

#define META(className, ...) \
using ThisClassType = className; \
inline static ExtendedCpp::Reflection::TypeInfo GetMetaInfo() \
{ \
    return ExtendedCpp::Reflection::TypeInfo(#className, typeid(ThisClassType), {__VA_ARGS__}); \
} \
inline static const ExtendedCpp::Reflection::TypeInfo MetaInfo = GetMetaInfo();

#endif