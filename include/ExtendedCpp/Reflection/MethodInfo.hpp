#ifndef ExtendedCpp_Reflection_MethodInfo_HPP
#define ExtendedCpp_Reflection_MethodInfo_HPP

#include <string>

import ExtendedCpp.Reflection.MethodInfo;

#define METHOD(methodName, ...) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    return std::apply([](auto&&... args) \
    { \
        using ReturnType = decltype(std::declval<ThisClassType>().methodName(args...)); \
        auto methodLambda = [](ThisClassType* object, std::any& tupleArgs) \
        { \
            return std::apply([object](auto&&... args) \
                { return object->methodName(args...); }, std::any_cast<std::tuple<__VA_ARGS__>>(tupleArgs)); \
        }; \
        using MethodType = decltype(methodLambda); \
        return std::make_shared<ExtendedCpp::Reflection::MethodInfo>(#methodName, \
            ExtendedCpp::Reflection::MethodInfo::Helper<ThisClassType, MethodType, ReturnType> \
                (std::move(methodLambda)), ExtendedCpp::Reflection::ToTypeIndexes<__VA_ARGS__>()); \
    }, std::tuple<__VA_ARGS__>()); \
}()

#endif