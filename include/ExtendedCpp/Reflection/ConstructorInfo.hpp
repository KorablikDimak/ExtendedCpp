#ifndef ExtendedCpp_Reflection_ConstructorInfo_HPP
#define ExtendedCpp_Reflection_ConstructorInfo_HPP

#include <string>

import ExtendedCpp.Reflection.ConstructorInfo;

#define CONSTRUCTOR(...) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    return std::make_shared<ExtendedCpp::Reflection::ConstructorInfo>(std::string(typeid(ThisClassType).name()), \
        ExtendedCpp::Reflection::ConstructorInfo::Helper<ThisClassType __VA_OPT__(,) __VA_ARGS__>(), \
        ExtendedCpp::Reflection::ToTypeIndexes<__VA_ARGS__>()); \
}()

#endif