module;

#include <string>

export module ExtendedCpp.Reflection.MemberInfo;

import ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
{
    class MemberInfo
    {
    protected:
        std::string _name;

    public:
        explicit MemberInfo(const std::string& name) noexcept
        {
            _name = name;
        }

        explicit MemberInfo(std::string&& name) noexcept
        {
            _name = std::move(name);
        }

        virtual ~MemberInfo() = default;

        [[nodiscard]]
        virtual std::string Name() const noexcept
        {
            return _name;
        }

        [[nodiscard]]
        virtual MemberType MemberType() const noexcept
        {
            return MemberType::Unknown;
        }
    };
}