#ifndef REFLECTION_STATIC_METHOD_INFO_H
#define REFLECTION_STATIC_METHOD_INFO_H

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class StaticMethodInfo final : public MemberInfo
    {
    private:
        const std::any _methodHelper;
        std::any (*const _method)(const std::any& helper, std::any&& args);
        const std::vector<std::type_index> _parameters;

        template<typename TMethod, typename TReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            const TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TMethod method) noexcept :
                _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& args) const
            {
                if constexpr (std::same_as<TReturnType, void>)
                {
                    _method(std::any_cast<TArgs>(std::move(args))...);
                    return;
                }
                else
                    return _method(std::any_cast<TArgs>(std::move(args))...);
            }
        };

    public:
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
                    if constexpr (std::same_as<void, typename std::remove_cvref_t<THelper>::ReturnType>)
                    {
                        std::any_cast<const THelper&>(helper).Invoke(std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(args)));
                }),
            _parameters(std::move(parameters)) {}

        /// @brief
        ~StaticMethodInfo() noexcept override = default;

        /// @brief 
        /// @tparam TResult 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename TResult, typename... TArgs>
        TResult Invoke(TArgs&&... args) const
        {
            if constexpr (std::same_as<TResult, void>)
            {
                _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
                return;
            }
            else
                return std::any_cast<TResult>(_method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename... TArgs>
        std::any Invoke(TArgs&&... args) const
        {
            return _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
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
        /// @tparam TReturnType
        /// @tparam TArgs
        /// @param name
        /// @param methodPtr
        /// @return
        template<typename TReturnType, typename... TArgs>
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TReturnType(*const methodPtr)(TArgs...)) noexcept
        {
            return std::make_shared<StaticMethodInfo>(std::move(name), Helper<decltype(methodPtr),
                TReturnType, TArgs...>(methodPtr), ToTypeIndexes<TArgs...>());
        }
    };
}

#define STATIC_METHOD(methodName, ...) \
[]() \
{ \
    using TupleType = std::tuple<__VA_ARGS__>; \
    constexpr std::size_t vaSize = std::tuple_size_v<TupleType>; \
    return []<std::size_t... Indexes>(std::index_sequence<Indexes...>) \
    { \
        using ReturnType = decltype(methodName(std::get<Indexes>(std::declval<TupleType>())...)); \
        return ExtendedCpp::Reflection::StaticMethodInfo::Create<ReturnType __VA_OPT__(,) __VA_ARGS__>(#methodName, &methodName); \
    }(std::make_index_sequence<vaSize>()); \
}()

#endif