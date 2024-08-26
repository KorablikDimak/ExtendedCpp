#ifndef Reflection_MethodInfo_H
#define Reflection_MethodInfo_H

#include <any>
#include <stdexcept>
#include <vector>

#include <Reflection/MemberInfo.h>
#include <Reflection/TypeTraits.h>

namespace Reflection
{
    class MethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(std::any& helper, std::any&& object, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TObject, typename TMethod, typename TReturnType>
        struct Helper final
        {
            TMethod _method;

            explicit Helper(TMethod&& method) noexcept :
                _method(std::move(method)) {}

            TReturnType Invoke(std::any&& object, std::any&& args)
            {
                return _method(std::any_cast<TObject*>(object), args);
            }
        };

        template<typename THelper>
        MethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](std::any& helper, std::any&& object, std::any&& args)
                { return std::any(std::any_cast<THelper&>(helper).Invoke(std::move(object), std::move(args))); }),
            _parameters(std::move(parameters)),
            MemberInfo(methodName) {}

        template<typename THelper>
        MethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](std::any& helper, std::any&& object, std::any&& args)
                { return std::any(std::any_cast<THelper&>(helper).Invoke(std::move(object), std::move(args))); }),
            _parameters(std::move(parameters)),
            MemberInfo(std::move(methodName)) {}

        ~MethodInfo() override = default;

        template<typename TObject, typename... TArgs>
        auto Invoke(TObject* object, TArgs... args)
        {
            if (object == nullptr) return std::any();
            return _method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        inline Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Method;
        }

        [[nodiscard]]
        inline std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
    };
}

#define METHOD(methodName, ...) \
[]()->std::shared_ptr<Reflection::MemberInfo> \
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
        return std::make_shared<Reflection::MethodInfo>(#methodName, \
            Reflection::MethodInfo::Helper<ThisClassType, MethodType, ReturnType> \
                (std::move(methodLambda)), Reflection::ToTypeIndexes<__VA_ARGS__>()); \
    }, std::tuple<__VA_ARGS__>()); \
}()

#endif