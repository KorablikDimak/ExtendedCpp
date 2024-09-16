#ifndef Reflection_MethodInfo_H
#define Reflection_MethodInfo_H

#include <any>
#include <stdexcept>
#include <vector>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/Concepts.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

namespace ExtendedCpp::Reflection
{
    enum class MethodCVQualifier
    {
        OnlyConst,
        OnlyNoConst,
        ConstNoConst
    };

    class MethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any _constMethodHelper;
        std::any (*_method)(const std::any& helper, std::any&& object, std::any&& args);
        std::any (*_constMethod)(const std::any& helper, std::any&& object, std::any&& args);
        std::vector<std::type_index> _parameters{};
        MethodCVQualifier _methodCvQualifier;

    public:
        template<typename TObject, typename TMethod, typename TReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TMethod&& method) noexcept :
                _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& object, std::any&& args) const
            {
                if constexpr (std::same_as<TReturnType, void>)
                    (std::any_cast<TObject*>(std::move(object))->*_method)(std::any_cast<TArgs>(std::move(args))...);
                else
                    return (std::any_cast<TObject*>(std::move(object))->*_method)(std::any_cast<TArgs>(std::move(args))...);
            }
        };

        template<typename TObject, typename TMethod, typename TReturnType, typename... TArgs>
        struct ConstHelper final
        {
        private:
            TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit ConstHelper(TMethod&& method) noexcept :
                _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& object, std::any&& args) const
            {
                if constexpr (std::same_as<TReturnType, void>)
                    (std::any_cast<const TObject*>(std::move(object))->*_method)(std::any_cast<TArgs>(std::move(args))...);
                else
                    return (std::any_cast<const TObject*>(std::move(object))->*_method)(std::any_cast<TArgs>(std::move(args))...);
            }
        };

        template<typename THelper, typename TConstHelper>
        MethodInfo(const std::string& methodName, THelper&& methodHelper, TConstHelper&& constMethodHelper,
                   std::vector<std::type_index>&& parameters, MethodCVQualifier methodCvQualifier) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _constMethodHelper(std::forward<TConstHelper>(constMethodHelper)),
            _method([](const std::any& helper, std::any&& object, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename THelper::ReturnType>)
                    {
                        std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args)));
                }),
            _constMethod([](const std::any& helper, std::any&& object, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename TConstHelper::ReturnType>)
                    {
                        std::any_cast<const TConstHelper&>(helper).Invoke(std::move(object), std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const TConstHelper&>(helper).Invoke(std::move(object), std::move(args)));
                }),
            _parameters(std::move(parameters)),
            _methodCvQualifier(methodCvQualifier),
            MemberInfo(methodName) {}

        template<typename THelper, typename TConstHelper>
        MethodInfo(std::string&& methodName, THelper&& methodHelper, TConstHelper&& constMethodHelper,
                   std::vector<std::type_index>&& parameters, MethodCVQualifier methodCvQualifier) noexcept :
            _methodHelper(std::forward<THelper>(methodHelper)),
            _constMethodHelper(std::forward<TConstHelper>(constMethodHelper)),
            _method([](const std::any& helper, std::any&& object, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename THelper::ReturnType>)
                    {
                        std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args)));
                }),
            _constMethod([](const std::any& helper, std::any&& object, std::any&& args)
                {
                    if constexpr (std::same_as<void, typename TConstHelper::ReturnType>)
                    {
                        std::any_cast<const TConstHelper&>(helper).Invoke(std::move(object), std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const TConstHelper&>(helper).Invoke(std::move(object), std::move(args)));
                }),
            _parameters(std::move(parameters)),
            _methodCvQualifier(methodCvQualifier),
            MemberInfo(std::move(methodName)) {}

        ~MethodInfo() override = default;

        template<typename TResult, typename TObject, typename... TArgs>
        TResult Invoke(TObject* object, TArgs&&... args) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            if constexpr (std::same_as<TResult, void>)
                _method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
            else
                return std::any_cast<TResult>(_method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename TResult, typename TObject, typename... TArgs>
        TResult Invoke(const TObject* object, TArgs&&... args) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            if (_methodCvQualifier == MethodCVQualifier::OnlyNoConst)
                throw std::runtime_error("Object has not const method " + _name);
            if constexpr (std::same_as<TResult, void>)
                _constMethod(_constMethodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
            else
                return std::any_cast<TResult>(_constMethod(_constMethodHelper, object, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename TObject, typename... TArgs>
        std::any Invoke(TObject* object, TArgs&&... args) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
        }

        template<typename TObject, typename... TArgs>
        std::any Invoke(const TObject* object, TArgs&&... args) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            if (_methodCvQualifier == MethodCVQualifier::OnlyNoConst)
                throw std::runtime_error("Object has not const method " + _name);
            return _constMethod(_constMethodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;
        [[nodiscard]]
        bool HasConstOverload() const noexcept;
        [[nodiscard]]
        bool HasNoConstOverload() const noexcept;
    };

    template<typename TObject, typename TConstReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(const std::string& name,
                                                 TConstReturnType(TObject::*constMethodPtr)(TArgs...) const) noexcept
    {
        return std::make_shared<MethodInfo>(name,
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::OnlyConst);
    }

    template<typename TObject, typename TReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(const std::string& name,
                                                 TReturnType(TObject::*methodPtr)(TArgs...)) noexcept
    {
        return std::make_shared<MethodInfo>(name,
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::OnlyNoConst);
    }

    template<typename TObject, typename TReturnType, typename TConstReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(const std::string& name,
                                                 TReturnType(TObject::*methodPtr)(TArgs...),
                                                 TConstReturnType(TObject::*constMethodPtr)(TArgs...) const) noexcept
    {
        return std::make_shared<MethodInfo>(name,
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::ConstNoConst);
    }

    template<typename TObject, typename TConstReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(std::string&& name,
                                                 TConstReturnType(TObject::*constMethodPtr)(TArgs...) const) noexcept
    {
        return std::make_shared<MethodInfo>(std::move(name),
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::OnlyConst);
    }

    template<typename TObject, typename TReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(std::string&& name,
                                                 TReturnType(TObject::*methodPtr)(TArgs...)) noexcept
    {
        return std::make_shared<MethodInfo>(std::move(name),
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::OnlyNoConst);
    }

    template<typename TObject, typename TReturnType, typename TConstReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateMethodInfo(std::string&& name,
                                                 TReturnType(TObject::*methodPtr)(TArgs...),
                                                 TConstReturnType(TObject::*constMethodPtr)(TArgs...) const) noexcept
    {
        return std::make_shared<MethodInfo>(std::move(name),
            MethodInfo::Helper<TObject, decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            MethodInfo::ConstHelper<TObject, decltype(constMethodPtr), TConstReturnType, TArgs...>(std::move(constMethodPtr)),
            ToTypeIndexes<TArgs...>(), MethodCVQualifier::ConstNoConst);
    }
}

#define METHOD(methodName, ...) \
[]() \
{ \
    using TupleType = std::tuple<__VA_ARGS__>; \
    constexpr std::size_t vaSize = std::tuple_size_v<TupleType>; \
    return []<std::size_t... Indexes>(std::index_sequence<Indexes...>) \
    { \
        if constexpr(!requires { std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...); }) \
        { \
            using ReturnType = decltype(std::declval<ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            ReturnType(ThisClassType::*methodPtr)(__VA_ARGS__) = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::CreateMethodInfo<ThisClassType, ReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, methodPtr); \
        } \
        else if constexpr(requires { static_cast<decltype(std::declval<ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)) \
                                (ThisClassType::*)(__VA_ARGS__)>(&ThisClassType::methodName); } && \
                          requires { static_cast<decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)) \
                                (ThisClassType::*)(__VA_ARGS__) const>(&ThisClassType::methodName); }) \
        { \
            using ReturnType = decltype(std::declval<ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            using ConstReturnType = decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            ReturnType(ThisClassType::*methodPtr)(__VA_ARGS__) = &ThisClassType::methodName; \
            ConstReturnType(ThisClassType::*constMethodPtr)(__VA_ARGS__) const = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::CreateMethodInfo<ThisClassType, ReturnType, ConstReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, methodPtr, constMethodPtr); \
        } \
        else \
        { \
            using ConstReturnType = decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            ConstReturnType(ThisClassType::*constMethodPtr)(__VA_ARGS__) const = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::CreateMethodInfo<ThisClassType, ConstReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, constMethodPtr); \
        } \
    }(std::make_index_sequence<vaSize>()); \
}()

#endif