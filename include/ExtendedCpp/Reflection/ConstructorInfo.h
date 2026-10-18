#ifndef REFLECTION_CONSTRUCTOR_INFO_H
#define REFLECTION_CONSTRUCTOR_INFO_H

#include <any>
#include <stdexcept>
#include <typeindex>
#include <vector>

#include <ExtendedCpp/Reflection/Concepts.h>
#include <ExtendedCpp/Reflection/MemberInfo.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any _constructorHelper;

        std::any (*_constructor)(const std::any& helper, std::any&& args);
        std::shared_ptr<void> (*_constructorNew)(const std::any& helper, std::any&& args){};

        std::any (*_fromAny)(const std::any& helper, const std::vector<std::any>& args){};
        std::shared_ptr<void> (*_fromAnyNew)(const std::any& helper, const std::vector<std::any>& args){};

        std::any (*_fromPtr)(const std::any& helper, const std::vector<std::shared_ptr<void>>& args){};
        std::shared_ptr<void> (*_fromPtrNew)(const std::any& helper, const std::vector<std::shared_ptr<void>>& args){};

        std::vector<std::type_index> _parameters;

    public:
        /// @brief 
        /// @tparam TClass 
        /// @tparam TArgs
        template<typename TClass, typename... TArgs>
        struct Helper final
        {
            using TupleArgs = std::tuple<TArgs...>;
            static constexpr std::size_t SIZE = std::tuple_size_v<TupleArgs>;

            /// @brief 
            /// @param args 
            /// @return
            TClass Create(std::any&& args) const
            {
                if constexpr (!SIZE)
                    return TClass();
                else
                    return std::apply([](TArgs&&... argsInner)
                        { return TClass(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            /// @brief 
            /// @param args 
            /// @return
            TClass* New(std::any&& args) const
            {
                if constexpr (!SIZE)
                    return new TClass();
                else
                    return std::apply([](TArgs&&... argsInner)
                        { return new TClass(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            /// @brief 
            /// @param args 
            /// @return
            TClass CreateFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (!SIZE)
                {
                    return TClass();
                }
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass
                    {
                        return TClass(std::any_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
            }

            /// @brief 
            /// @param args 
            /// @return
            TClass CreateFromPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (!SIZE)
                {
                    return TClass();
                }
                else if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass
                    {
                        return TClass(std::static_pointer_cast<typename std::tuple_element_t<Index, TupleArgs>::element_type>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
            }

            /// @brief 
            /// @param args 
            /// @return
            TClass* NewFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (!SIZE)
                {
                    return new TClass();
                }
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass*
                    {
                        return new TClass(std::any_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
            }

            /// @brief 
            /// @param args 
            /// @return
            TClass* NewFromPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (!SIZE)
                {
                    return new TClass();
                }
                else if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass*
                    {
                        return new TClass(std::static_pointer_cast<typename std::tuple_element_t<Index, TupleArgs>::element_type>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
            }
        };

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(push)
    #pragma warning(disable : 4702)
#endif

        /// @brief 
        /// @tparam THelper 
        /// @param constructorName 
        /// @param constructorHelper 
        /// @param parameters
        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper&& constructorHelper, std::vector<std::type_index>&& parameters) noexcept :
            MemberInfo(std::move(constructorName)),
            _constructorHelper(std::forward<THelper>(constructorHelper)),
            _constructor([](const std::any& helper, std::any&& args)
                { return std::any(std::any_cast<const THelper&>(helper).Create(std::move(args))); }),
            _constructorNew([](const std::any& helper, std::any&& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).New(std::move(args))); }),
            _fromAny([](const std::any& helper, const std::vector<std::any>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromAny(args)); }),
            _fromAnyNew([](const std::any& helper, const std::vector<std::any>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromAny(args)); }),
            _fromPtr([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromPtr(args)); }),
            _fromPtrNew([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromPtr(args)); }),
            _parameters(std::move(parameters)) {}

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(pop)
#endif

        /// @brief
        ~ConstructorInfo() override = default;

        /// @brief 
        /// @tparam TTarget 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename TTarget, typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        TTarget Create(TArgs&&... args) const
        {
            return std::any_cast<TTarget>
                    (_constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        std::any Create(TArgs&&... args) const
        {
            return _constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename TTarget, typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        std::shared_ptr<TTarget> New(TArgs&&... args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        std::shared_ptr<void> New(TArgs&&... args) const
        {
            return _constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        TTarget Create(const std::vector<std::any>& args) const
        {
            return std::any_cast<TTarget>
                    (_fromAny(_constructorHelper, args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::vector<std::any>& args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_fromAnyNew(_constructorHelper, args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        TTarget Create(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::any_cast<TTarget>
                    (_fromPtr(_constructorHelper, args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_fromPtrNew(_constructorHelper, args));
        }

        /// @brief 
        /// @param args 
        /// @return
        [[nodiscard]]
        std::any Create(const std::vector<std::any>& args) const;

        /// @brief 
        /// @param args 
        /// @return
        [[nodiscard]]
        std::shared_ptr<void> New(const std::vector<std::any>& args) const;

        /// @brief 
        /// @param args 
        /// @return
        [[nodiscard]]
        std::any Create(const std::vector<std::shared_ptr<void>>& args) const;

        /// @brief 
        /// @param args 
        /// @return
        [[nodiscard]]
        std::shared_ptr<void> New(const std::vector<std::shared_ptr<void>>& args) const;

        /// @brief 
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;
    };
}

#define CONSTRUCTOR(...) \
[]()->std::shared_ptr<ExtendedCpp::Reflection::MemberInfo> \
{ \
    return std::make_shared<ExtendedCpp::Reflection::ConstructorInfo>(std::string(typeid(ThisClassType).name()), \
        ExtendedCpp::Reflection::ConstructorInfo::Helper<ThisClassType __VA_OPT__(,) __VA_ARGS__>(), \
        ExtendedCpp::Reflection::ToTypeIndexes<__VA_ARGS__>()); \
}()

#endif