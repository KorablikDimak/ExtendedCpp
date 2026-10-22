#ifndef REFLECTION_MEMBER_INFO_H
#define REFLECTION_MEMBER_INFO_H

#include <string>

#include <ExtendedCpp/Reflection/MemberType.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class MemberInfo
    {
    private:
        const std::string _name;

    public:
        /// @brief 
        /// @param name
        explicit MemberInfo(std::string&& name) noexcept;

        /// @brief
        virtual ~MemberInfo() noexcept = default;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::string_view Name() const noexcept;

        /// @brief 
        /// @return
        [[nodiscard]]
        virtual MemberType GetMemberType() const noexcept = 0;
    };
}

#endif