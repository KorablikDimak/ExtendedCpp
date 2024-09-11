#ifndef ExtendedCpp_Reflection_StaticFieldInfo_HPP
#define ExtendedCpp_Reflection_StaticFieldInfo_HPP

#include <string>

import ExtendedCpp.Reflection.StaticFieldInfo;

#define STATIC_FIELD(name) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    using FieldType = decltype(name); \
    auto fieldGetter = []()->FieldType* \
    { \
        return &(name); \
    }; \
    return std::make_shared<ExtendedCpp::Reflection::StaticFieldInfo>(#name, typeid(FieldType), \
        ExtendedCpp::Reflection::StaticFieldInfo::Helper<FieldType, decltype(fieldGetter)>(std::move(fieldGetter))); \
}()

#endif