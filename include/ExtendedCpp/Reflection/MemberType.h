#ifndef REFLECTION_MEMBER_TYPE_H
#define REFLECTION_MEMBER_TYPE_H

#include <cstdint>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    enum class MemberType : std::uint8_t
    {
        Unknown,
        Field,
        Method,
        Constructor,
        StaticMethod,
        StaticField
    };
}

#endif