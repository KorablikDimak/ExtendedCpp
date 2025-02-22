#ifndef Reflection_MemberType_H
#define Reflection_MemberType_H

/// @brief 
namespace ExtendedCpp::Reflection
{
    /// @brief 
    enum class MemberType
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