#ifndef ExtendedCpp_Reflection_StaticMethodInfo_HPP
#define ExtendedCpp_Reflection_StaticMethodInfo_HPP

#include <string>

import ExtendedCpp.Reflection.StaticMethodInfo;

#define STATIC_METHOD(methodName, ...) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    auto methodLambda = [](std::any&& tupleArgs) \
    { \
        return std::apply([](auto&&... args) \
            { return ThisClassType::methodName(args...); }, std::any_cast<std::tuple<__VA_ARGS__>>(tupleArgs)); \
    }; \
    using MethodType = decltype(methodLambda);  \
    using ReturnType = decltype(std::declval<MethodType>()(std::declval<std::any>())); \
    return std::make_shared<Reflection::StaticMethodInfo>(#methodName, \
        ExtendedCpp::Reflection::StaticMethodInfo::Helper<MethodType, ReturnType> \
            (std::move(methodLambda)), ExtendedCpp::Reflection::ToTypeIndexes<__VA_ARGS__>()); \
}()

#endif