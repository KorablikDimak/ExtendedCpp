#ifndef ExtendedCpp_Reflection_FieldInfo_HPP
#define ExtendedCpp_Reflection_FieldInfo_HPP

#include <string>

import ExtendedCpp.Reflection.FieldInfo;

#define FIELD(name) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    using FieldType = decltype(std::declval<ThisClassType>().name); \
    auto fieldGetter = [](ThisClassType* object)->FieldType* \
    { \
        return &(object->name);\
    }; \
    return std::make_shared<ExtendedCpp::Reflection::FieldInfo>(#name, typeid(FieldType), \
        ExtendedCpp::Reflection::FieldInfo::Helper<ThisClassType, FieldType, decltype(fieldGetter)>(std::move(fieldGetter))); \
}()

#endif