#ifndef Reflection_MemberType_H
#define Reflection_MemberType_H

namespace ExtendedCpp::Reflection
{
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