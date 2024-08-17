#ifndef Reflection_MemberInfo_H
#define Reflection_MemberInfo_H

#include <string>

#include <Reflection/MemberType.h>

namespace Reflection
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
        virtual MemberType MemberType() const noexcept;
    };
}

#endif