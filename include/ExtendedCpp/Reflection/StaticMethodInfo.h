#ifndef REFLECTION_STATIC_METHOD_INFO_H
#define REFLECTION_STATIC_METHOD_INFO_H

#include <any>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <ExtendedCpp/Reflection/Concepts.h>
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
        const std::vector<std::type_index> _parameters;

        std::any (*const _method)(const std::any& helper, std::any&& args);
        std::any (*const _methodWithAny)(const std::any& helper, std::span<const std::any> args);
        std::any (*const _methodWithPtr)(const std::any& helper, std::span<const std::shared_ptr<void>> args);

        template<typename TReturnType, typename... TArgs>
        struct Helper final
        {
        private:
            TReturnType (*const _methodPtr)(TArgs...);
            using TupleArgs = std::tuple<TArgs...>;
            static constexpr std::size_t SIZE = sizeof...(TArgs);

        public:
            using ReturnType = TReturnType;

            explicit Helper(TReturnType (*const methodPtr)(TArgs...)) noexcept :
                _methodPtr(methodPtr) {}

            TReturnType Invoke(std::any&& args) const
            {
                return _methodPtr(std::any_cast<TArgs>(std::move(args))...);
            }

            TReturnType Invoke(const std::span<const std::any> args) const
            {
                if (args.size() != SIZE)
                    throw std::range_error("Incorrect args size in constructor");

                return [this, args]<std::size_t... Index>(std::index_sequence<Index...>)
                {
                    return _methodPtr(std::any_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                }(std::make_index_sequence<SIZE>());
            }

            TReturnType Invoke(const std::span<const std::shared_ptr<void>> args) const
            {
                if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                {
                    throw std::runtime_error("Method can not invoke with shared_ptr args");
                }
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [this, args]<std::size_t... Index>(std::index_sequence<Index...>)
                    {
                        return _methodPtr(std::static_pointer_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
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
            _parameters(std::move(parameters)),

            _method([](const std::any& helper, std::any&& args)
                {
                    if constexpr (std::is_void_v<typename std::remove_cvref_t<THelper>::ReturnType>)
                    {
                        std::any_cast<THelper>(helper).Invoke(std::move(args));
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<THelper>(helper).Invoke(std::move(args)));
                }),
            _methodWithAny([](const std::any& helper, const std::span<const std::any> args)
                {
                    if constexpr (std::is_void_v<typename std::remove_cvref_t<THelper>::ReturnType>)
                    {
                        std::any_cast<THelper>(helper).Invoke(args);
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<THelper>(helper).Invoke(args));
                }),
            _methodWithPtr([](const std::any& helper, const std::span<const std::shared_ptr<void>> args)
                {
                    if constexpr (std::is_void_v<typename std::remove_cvref_t<THelper>::ReturnType>)
                    {
                        std::any_cast<THelper>(helper).Invoke(args);
                        return std::any();
                    }
                    else
                        return std::any(std::any_cast<THelper>(helper).Invoke(args));
                }) {}

        /// @brief
        ~StaticMethodInfo() noexcept override = default;

        /// @brief 
        /// @tparam TResult 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename TResult, Concepts::ValidArgs... TArgs>
        TResult Invoke(TArgs&&... args) const
        {
            return std::any_cast<TResult>(_method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<Concepts::ValidArgs... TArgs>
        [[nodiscard]]
        std::any Invoke(TArgs&&... args) const
        {
            return _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief
        /// @tparam TResult
        /// @param args
        /// @return
        template<typename TResult>
        TResult Invoke(const std::span<const std::any> args) const
        {
            return std::any_cast<TResult>(_methodWithAny(_methodHelper, args));
        }

        /// @brief
        /// @param args
        /// @return
        [[nodiscard]]
        std::any Invoke(std::span<const std::any> args) const;

        /// @brief
        /// @tparam TResult
        /// @param args
        /// @return
        template<typename TResult>
        TResult Invoke(const std::span<const std::shared_ptr<void>> args) const
        {
            return std::any_cast<TResult>(_methodWithPtr(_methodHelper, args));
        }

        /// @brief
        /// @param args
        /// @return
        [[nodiscard]]
        std::any Invoke(std::span<const std::shared_ptr<void>> args) const;

        /// @brief 
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::span<const std::type_index> Parameters() const noexcept;

        /// @brief
        /// @tparam TReturnType
        /// @tparam TArgs
        /// @param name
        /// @param methodPtr
        /// @return
        template<typename TReturnType, typename... TArgs>
        [[nodiscard]]
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TReturnType (*const methodPtr)(TArgs...)) noexcept
        {
            return std::make_shared<StaticMethodInfo>(std::move(name), Helper<TReturnType, TArgs...>(methodPtr), ToTypeIndexes<TArgs...>());
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