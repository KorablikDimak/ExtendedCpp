#ifndef Reflection_StaticMethodInfo_H
#define Reflection_StaticMethodInfo_H

#include <any>
#include <stdexcept>
#include <utility>
#include <vector>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

namespace ExtendedCpp::Reflection
{
    class StaticMethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(const std::any& helper, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TMethod, typename TReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TMethod&& method) noexcept :
                _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& args) const
            {
                if constexpr (std::same_as<TReturnType, void>)
                    _method(std::any_cast<TArgs>(std::move(args))...);
                else
                    return _method(std::any_cast<TArgs>(std::move(args))...);
            }
        };

        template<typename THelper>
        StaticMethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](const std::any& helper, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename THelper::ReturnType>)
                    {
                        std::any_cast<const THelper&>(helper).Invoke(std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(args)));
                }),
            _parameters(std::move(parameters)),
            MemberInfo(methodName) {}

        template<typename THelper>
        StaticMethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _method([](const std::any& helper, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename THelper::ReturnType>)
                    {
                        std::any_cast<const THelper&>(helper).Invoke(std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(args)));
                }),
            _parameters(std::move(parameters)),
            MemberInfo(std::move(methodName)) {}

        ~StaticMethodInfo() override = default;

        template<typename TResult, typename... TArgs>
        TResult Invoke(TArgs&&... args) const
        {
            if constexpr (std::same_as<TResult, void>)
                _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
            else
                return std::any_cast<TResult>(_method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename... TArgs>
        std::any Invoke(TArgs&&... args) const
        {
            return _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;
    };

    template<typename TReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateStaticMethodInfo(const std::string& name,
                                                       TReturnType(*methodPtr)(TArgs...)) noexcept
    {
        return std::make_shared<StaticMethodInfo>(name,
            StaticMethodInfo::Helper<decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            ToTypeIndexes<TArgs...>());
    }

    template<typename TReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateStaticMethodInfo(std::string&& name,
                                                       TReturnType(*methodPtr)(TArgs...)) noexcept
    {
        return std::make_shared<StaticMethodInfo>(std::move(name),
            StaticMethodInfo::Helper<decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            ToTypeIndexes<TArgs...>());
    }
}

#define STATIC_METHOD(methodName, ...) \
[]() \
{ \
    return std::apply([](auto&&... args) \
    { \
        using ReturnType = decltype(methodName(args...)); \
        return ExtendedCpp::Reflection::CreateStaticMethodInfo<ReturnType __VA_OPT__(,) __VA_ARGS__>(#methodName, &methodName); \
    }, std::tuple<__VA_ARGS__>()); \
}()

#endif