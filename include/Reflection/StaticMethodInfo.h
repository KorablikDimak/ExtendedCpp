#ifndef Reflection_StaticMethodInfo_H
#define Reflection_StaticMethodInfo_H

#include <any>
#include <stdexcept>
#include <utility>
#include <vector>

#include <Reflection/MemberInfo.h>
#include <Reflection/TypeTraits.h>

namespace Reflection
{
    #define STATIC_METHOD(methodName, ...) \
    []()->std::shared_ptr<MemberInfo> \
    { \
        auto methodLambda = [](std::any&& tupleArgs) \
        { \
            return std::apply([](auto&&... args) \
                { return ThisClassType::methodName(args...); }, std::any_cast<std::tuple<__VA_ARGS__>>(tupleArgs)); \
        }; \
        using MethodType = decltype(methodLambda);  \
        using ReturnType = decltype(std::declval<MethodType>()(std::declval<std::any>())); \
        return std::make_shared<StaticMethodInfo>(#methodName, \
            StaticMethodInfo::Helper<MethodType, ReturnType> \
            (std::move(methodLambda)), ToTypeIndexes<__VA_ARGS__>()); \
    }()

    class StaticMethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(std::any& helper, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TMethod, typename TReturnType>
        struct Helper final
        {
            TMethod _method;

            explicit Helper(TMethod&& method) : _method(std::move(method)) {}

            TReturnType Invoke(std::any&& args)
            {
                return _method(std::move(args));
            }
        };

        template<typename THelper>
        StaticMethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](std::any& helper, std::any&& args)
                { return std::any(std::any_cast<THelper&>(helper).Invoke(std::move(args))); }),
            _parameters(std::move(parameters)),
            MemberInfo(methodName) {}

        template<typename THelper>
        StaticMethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](std::any& helper, std::any&& args)
                { return std::any(std::any_cast<THelper&>(helper).Invoke(std::move(args))); }),
            _parameters(std::move(parameters)),
            MemberInfo(std::move(methodName)) {}

        ~StaticMethodInfo() override = default;

        template<typename... TArgs>
        auto Invoke(TArgs... args){ return _method(_methodHelper, std::make_tuple(std::forward(args)...)); }

        [[nodiscard]]
        inline Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::StaticMethod;
        }

        [[nodiscard]]
        inline std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
    };
}

#endif