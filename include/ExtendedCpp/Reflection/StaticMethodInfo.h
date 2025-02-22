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
    /// @brief 
    class StaticMethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(const std::any& helper, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        /// @brief 
        /// @tparam TMethod 
        /// @tparam TReturnType 
        /// @tparam ...TArgs 
        template<typename TMethod, typename TReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            TMethod _method;

        public:
            /// @brief 
            using ReturnType = TReturnType;

            /// @brief 
            /// @param method 
            explicit Helper(TMethod&& method) noexcept :
                _method(std::forward<TMethod>(method)) {}

            /// @brief 
            /// @param args 
            /// @return 
            TReturnType Invoke(std::any&& args) const
            {
                if constexpr (std::same_as<TReturnType, void>)
                    _method(std::any_cast<TArgs>(std::move(args))...);
                else
                    return _method(std::any_cast<TArgs>(std::move(args))...);
            }
        };

        /// @brief 
        /// @tparam THelper 
        /// @param methodName 
        /// @param methodHelper 
        /// @param parameters 
        template<typename THelper>
        StaticMethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            MemberInfo(methodName),
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
            _parameters(std::move(parameters)) {}

        /// @brief 
        /// @tparam THelper 
        /// @param methodName 
        /// @param methodHelper 
        /// @param parameters 
        template<typename THelper>
        StaticMethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
            MemberInfo(std::move(methodName)),
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
            _parameters(std::move(parameters)) {}

        /// @brief 
        ~StaticMethodInfo() override = default;

        /// @brief 
        /// @tparam TResult 
        /// @tparam ...TArgs 
        /// @param ...args 
        /// @return 
        template<typename TResult, typename... TArgs>
        TResult Invoke(TArgs&&... args) const
        {
            if constexpr (std::same_as<TResult, void>)
                _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
            else
                return std::any_cast<TResult>(_method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam ...TArgs 
        /// @param ...args 
        /// @return 
        template<typename... TArgs>
        std::any Invoke(TArgs&&... args) const
        {
            return _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @return 
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;
    };

    /// @brief 
    /// @tparam TReturnType 
    /// @tparam ...TArgs 
    /// @param name 
    /// @param methodPtr 
    /// @return 
    template<typename TReturnType, typename... TArgs>
    std::shared_ptr<MemberInfo> CreateStaticMethodInfo(const std::string& name,
                                                       TReturnType(*methodPtr)(TArgs...)) noexcept
    {
        return std::make_shared<StaticMethodInfo>(name,
            StaticMethodInfo::Helper<decltype(methodPtr), TReturnType, TArgs...>(std::move(methodPtr)),
            ToTypeIndexes<TArgs...>());
    }

    /// @brief 
    /// @tparam TReturnType 
    /// @tparam ...TArgs 
    /// @param name 
    /// @param methodPtr 
    /// @return 
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
    using TupleType = std::tuple<__VA_ARGS__>; \
    constexpr std::size_t vaSize = std::tuple_size_v<TupleType>; \
    return []<std::size_t... Indexes>(std::index_sequence<Indexes...>) \
    { \
        using ReturnType = decltype(methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
        return ExtendedCpp::Reflection::CreateStaticMethodInfo<ReturnType __VA_OPT__(,) __VA_ARGS__>(#methodName, &methodName); \
    }(std::make_index_sequence<vaSize>()); \
}()

#endif