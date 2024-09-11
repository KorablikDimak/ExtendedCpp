export module ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
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