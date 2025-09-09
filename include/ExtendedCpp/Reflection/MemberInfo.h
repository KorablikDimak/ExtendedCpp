#ifndef Reflection_MemberInfo_H
#define Reflection_MemberInfo_H

#include <string>

#include <ExtendedCpp/Reflection/MemberType.h>

/// @brief 
namespace ExtendedCpp::Reflection
{
    /// @brief 
    class MemberInfo
    {
    protected:
        std::string _name;

    public:
        /// @brief 
        /// @param name 
        explicit MemberInfo(std::string&& name) noexcept;

        /// @brief 
        virtual ~MemberInfo() = default;

        /// @brief 
        /// @return 
        [[nodiscard]]
        virtual std::string Name() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        virtual MemberType MemberType() const noexcept;
    };
}

#endif