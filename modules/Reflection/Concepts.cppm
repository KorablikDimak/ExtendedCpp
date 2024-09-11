export module ExtendedCpp.Reflection.Concepts;

export namespace ExtendedCpp::Reflection::Concepts
{
    template<typename T>
    concept HasMetaInfo = requires
    {
        T::MetaInfo;
    };
}