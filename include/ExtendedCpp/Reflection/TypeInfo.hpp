#ifndef ExtendedCpp_Reflection_TypeInfo_HPP
#define ExtendedCpp_Reflection_TypeInfo_HPP

#include <string>

import ExtendedCpp.Reflection.TypeInfo;

#define META(className, ...) \
using ThisClassType = className; \
static ExtendedCpp::Reflection::TypeInfo GetMetaInfo() \
{ \
    return ExtendedCpp::Reflection::TypeInfo(#className, typeid(ThisClassType), {__VA_ARGS__}); \
} \
inline static const ExtendedCpp::Reflection::TypeInfo MetaInfo = GetMetaInfo();

#endif