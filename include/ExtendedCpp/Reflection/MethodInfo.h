#ifndef REFLECTION_METHOD_INFO_H
#define REFLECTION_METHOD_INFO_H

#include <any>
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class MethodInfo final : public MemberInfo
    {
    private:
        enum class MethodCvQualifier : std::uint8_t
        {
            OnlyConst,
            OnlyNoConst,
            ConstNoConst
        };

        const std::any _methodHelper;
        const std::vector<std::type_index> _parameters;
        const MethodCvQualifier _methodCvQualifier;

        std::any (*const _method)(const std::any& helper, std::any&& object, std::any&& args);
        std::any (*const _methodFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& args);
        std::any (*const _constMethodFromPtr)(const std::any& helper, const std::shared_ptr<void>& object, std::any&& args);

        template<typename TObject, typename TReturnType, typename TConstReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            TReturnType (TObject::*const _methodPtr)(TArgs...);
            TConstReturnType (TObject::*const _constMethodPtr)(TArgs...) const;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TReturnType (TObject::*const methodPtr)(TArgs...), TConstReturnType (TObject::*const constMethodPtr)(TArgs...) const) noexcept :
                _methodPtr(methodPtr),
                _constMethodPtr(constMethodPtr) {}

            std::any Invoke(std::any&& object, std::any&& args) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    if (_methodPtr)
                    {
                        if constexpr (std::is_void_v<TReturnType>)
                        {
                            (ptr->*_methodPtr)(std::any_cast<TArgs>(std::move(args))...);
                            return {};
                        }
                        else
                            return std::any((ptr->*_methodPtr)(std::any_cast<TArgs>(std::move(args))...));
                    }

                    if (_constMethodPtr)
                    {
                        if constexpr (std::is_void_v<TConstReturnType>)
                        {
                            (ptr->*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...);
                            return {};
                        }
                        else
                            return std::any((ptr->*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...));
                    }

                    throw std::domain_error(std::format("Object {} has not methods.", object.type().name()));
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TObject& ref = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get();

                    if (_methodPtr)
                    {
                        if constexpr (std::is_void_v<TReturnType>)
                        {
                            (ref.*_methodPtr)(std::any_cast<TArgs>(std::move(args))...);
                            return {};
                        }
                        else
                            return std::any((ref.*_methodPtr)(std::any_cast<TArgs>(std::move(args))...));
                    }

                    if (_constMethodPtr)
                    {
                        if constexpr (std::is_void_v<TConstReturnType>)
                        {
                            (ref.*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...);
                            return {};
                        }
                        else
                            return std::any((ref.*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...));
                    }

                    throw std::domain_error(std::format("Object {} has not methods.", object.type().name()));
                }

                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    if (!_constMethodPtr)
                        throw std::domain_error(std::format("Object {} is const, but has not const method.", object.type().name()));

                    if constexpr (std::is_void_v<TConstReturnType>)
                    {
                        (ptr->*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...);
                        return {};
                    }
                    else
                        return std::any((ptr->*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...));
                }

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                {
                    const TObject& ref = std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get();

                    if (!_constMethodPtr)
                        throw std::domain_error(std::format("Object {} is const, but has not const method.", object.type().name()));

                    if constexpr (std::is_void_v<TConstReturnType>)
                    {
                        (ref.*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...);
                        return {};
                    }
                    else
                        return std::any((ref.*_constMethodPtr)(std::any_cast<TArgs>(std::move(args))...));
                }

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {}, {}, {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(const TObject*).name(),
                    typeid(std::reference_wrapper<TObject>).name(), typeid(std::reference_wrapper<const TObject>).name()));
            }

            TReturnType Invoke(std::shared_ptr<void>& object, std::any&& args) const
            {

            }

            TConstReturnType Invoke(const std::shared_ptr<void>& object, std::any&& args) const
            {

            }
        };

    public:
        /// @brief 
        /// @tparam THelper
        /// @param methodName 
        /// @param methodHelper
        /// @param parameters 
        /// @param methodCvQualifier
        template<typename THelper>
        MethodInfo(std::string&& methodName, THelper&& methodHelper,
                   std::vector<std::type_index>&& parameters, const MethodCvQualifier methodCvQualifier) noexcept :
            MemberInfo(std::move(methodName)),
            _methodHelper(std::forward<THelper>(methodHelper)),
            _parameters(std::move(parameters)),
            _methodCvQualifier(methodCvQualifier),

            _method([](const std::any& helper, std::any&& object, std::any&& args)
                { return std::any_cast<THelper>(helper).Invoke(std::move(object), std::move(args)); }) {}

        /// @brief
        ~MethodInfo() noexcept override = default;

        /// @brief 
        /// @tparam TResult
        /// @tparam TArgs
        /// @param object 
        /// @param args
        /// @return
        template<typename TResult, typename... TArgs>
        TResult Invoke(std::any object, TArgs&&... args) const
        {
            return std::any_cast<TResult>(_method(_methodHelper, std::move(object), std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief
        /// @tparam TArgs
        /// @param object 
        /// @param args
        /// @return
        template<typename... TArgs>
        std::any Invoke(std::any object, TArgs&&... args) const
        {
            return _method(_methodHelper, std::move(object), std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;

        /// @brief 
        /// @return
        [[nodiscard]]
        bool HasConstOverload() const noexcept;

        /// @brief 
        /// @return
        [[nodiscard]]
        bool HasNoConstOverload() const noexcept;

        /// @brief
        /// @tparam TObject
        /// @tparam TConstReturnType
        /// @tparam TArgs
        /// @param name
        /// @param constMethodPtr
        /// @return
        template<typename TObject, typename TConstReturnType, typename... TArgs>
        static std::shared_ptr<MemberInfo> Create(std::string&& name,
                                                  TConstReturnType (TObject::*const constMethodPtr)(TArgs...) const) noexcept
        {
            return std::make_shared<MethodInfo>(std::move(name),
                MethodInfo::Helper<TObject, void, TConstReturnType, TArgs...>(nullptr, constMethodPtr),
                ToTypeIndexes<TArgs...>(), MethodCvQualifier::OnlyConst);
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TReturnType
        /// @tparam TArgs
        /// @param name
        /// @param methodPtr
        /// @return
        template<typename TObject, typename TReturnType, typename... TArgs>
        static std::shared_ptr<MemberInfo> Create(std::string&& name,
                                                  TReturnType (TObject::*const methodPtr)(TArgs...)) noexcept
        {
            return std::make_shared<MethodInfo>(std::move(name),
                MethodInfo::Helper<TObject, TReturnType, void, TArgs...>(methodPtr, nullptr),
                ToTypeIndexes<TArgs...>(), MethodCvQualifier::OnlyNoConst);
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TReturnType
        /// @tparam TConstReturnType
        /// @tparam TArgs
        /// @param name
        /// @param methodPtr
        /// @param constMethodPtr
        /// @return
        template<typename TObject, typename TReturnType, typename TConstReturnType, typename... TArgs>
        static std::shared_ptr<MemberInfo> Create(std::string&& name,
                                                  TReturnType (TObject::*const methodPtr)(TArgs...),
                                                  TConstReturnType (TObject::*const constMethodPtr)(TArgs...) const) noexcept
        {
            return std::make_shared<MethodInfo>(std::move(name),
                MethodInfo::Helper<TObject, TReturnType, TConstReturnType, TArgs...>(methodPtr, constMethodPtr),
                ToTypeIndexes<TArgs...>(), MethodCvQualifier::ConstNoConst);
        }
    };
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
            ReturnType(ThisClassType::*const methodPtr)(__VA_ARGS__) = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::MethodInfo::Create<ThisClassType, ReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, methodPtr); \
        } \
        else if constexpr(requires { static_cast<decltype(std::declval<ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)) \
                                (ThisClassType::*)(__VA_ARGS__)>(&ThisClassType::methodName); } && \
                          requires { static_cast<decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)) \
                                (ThisClassType::*)(__VA_ARGS__) const>(&ThisClassType::methodName); }) \
        { \
            using ReturnType = decltype(std::declval<ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            using ConstReturnType = decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            ReturnType(ThisClassType::*const methodPtr)(__VA_ARGS__) = &ThisClassType::methodName; \
            ConstReturnType(ThisClassType::*const constMethodPtr)(__VA_ARGS__) const = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::MethodInfo::Create<ThisClassType, ReturnType, ConstReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, methodPtr, constMethodPtr); \
        } \
        else \
        { \
            using ConstReturnType = decltype(std::declval<const ThisClassType>().methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
            ConstReturnType(ThisClassType::*const constMethodPtr)(__VA_ARGS__) const = &ThisClassType::methodName; \
            return ExtendedCpp::Reflection::MethodInfo::Create<ThisClassType, ConstReturnType __VA_OPT__(,) __VA_ARGS__> \
                (#methodName, constMethodPtr); \
        } \
    }(std::make_index_sequence<vaSize>()); \
}()

#endif