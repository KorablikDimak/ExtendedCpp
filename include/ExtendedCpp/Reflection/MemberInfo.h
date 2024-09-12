#ifndef Reflection_MemberInfo_H
#define Reflection_MemberInfo_H

#include <string>

#include <ExtendedCpp/Reflection/MemberType.h>

namespace ExtendedCpp::Reflection
{
    class MemberInfo
    {
    protected:
        std::string _name;

    public:
        explicit MemberInfo(const std::string& name) noexcept;
        explicit MemberInfo(std::string&& name) noexcept;
        virtual ~MemberInfo() = default;

        [[nodiscard]]
        virtual std::string Name() const noexcept;
        [[nodiscard]]
        virtual Reflection::MemberType MemberType() const noexcept;
    };
}

#endif