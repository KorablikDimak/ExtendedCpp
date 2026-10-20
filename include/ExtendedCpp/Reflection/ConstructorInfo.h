#ifndef REFLECTION_CONSTRUCTOR_INFO_H
#define REFLECTION_CONSTRUCTOR_INFO_H

#include <any>
#include <stdexcept>
#include <typeindex>
#include <vector>

#include <ExtendedCpp/Reflection/Concepts.h>
#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any (*const _constructor)(std::any&& args);
        std::shared_ptr<void> (*const _constructorNew)(std::any&& args){};

        std::any (*const _fromAny)(const std::vector<std::any>& args){};
        std::shared_ptr<void> (*const _fromAnyNew)(const std::vector<std::any>& args){};

        std::any (*const _fromPtr)(const std::vector<std::shared_ptr<void>>& args){};
        std::shared_ptr<void> (*const _fromPtrNew)(const std::vector<std::shared_ptr<void>>& args){};

        const std::vector<std::type_index> _parameters;

        template<typename TClass, typename... TArgs>
        struct Helper final
        {
        private:
            using TupleArgs = std::tuple<TArgs...>;
            static constexpr std::size_t SIZE = std::tuple_size_v<TupleArgs>;

        public:
            static TClass Create(std::any&& args)
            {
                if constexpr (!SIZE)
                    return TClass();
                else
                    return std::apply([](TArgs&&... argsInner)
                        { return TClass(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            static TClass* New(std::any&& args)
            {
                if constexpr (!SIZE)
                    return new TClass();
                else
                    return std::apply([](TArgs&&... argsInner)
                        { return new TClass(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            static TClass CreateFromAny(const std::vector<std::any>& args)
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

            static TClass CreateFromPtr(const std::vector<std::shared_ptr<void>>& args)
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

            static TClass* NewFromAny(const std::vector<std::any>& args)
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

            static TClass* NewFromPtr(const std::vector<std::shared_ptr<void>>& args)
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

    public:

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(push)
    #pragma warning(disable : 4702)
#endif

        /// @brief 
        /// @tparam THelper 
        /// @param constructorName
        /// @param parameters
        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper&&, std::vector<std::type_index>&& parameters) noexcept :
            MemberInfo(std::move(constructorName)),
            _constructor([](std::any&& args)
                { return std::any(std::remove_cvref_t<THelper>::Create(std::move(args))); }),
            _constructorNew([](std::any&& args)
                { return std::shared_ptr<void>(std::remove_cvref_t<THelper>::New(std::move(args))); }),
            _fromAny([](const std::vector<std::any>& args)
                { return std::any(std::remove_cvref_t<THelper>::CreateFromAny(args)); }),
            _fromAnyNew([](const std::vector<std::any>& args)
                { return std::shared_ptr<void>(std::remove_cvref_t<THelper>::NewFromAny(args)); }),
            _fromPtr([](const std::vector<std::shared_ptr<void>>& args)
                { return std::any(std::remove_cvref_t<THelper>::CreateFromPtr(args)); }),
            _fromPtrNew([](const std::vector<std::shared_ptr<void>>& args)
                { return std::shared_ptr<void>(std::remove_cvref_t<THelper>::NewFromPtr(args)); }),
            _parameters(std::move(parameters)) {}

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(pop)
#endif

        /// @brief
        ~ConstructorInfo() noexcept override = default;

        /// @brief 
        /// @tparam TTarget 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename TTarget, typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        TTarget Create(TArgs&&... args) const
        {
            return std::any_cast<TTarget>(_constructor(std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        std::any Create(TArgs&&... args) const
        {
            return _constructor(std::make_tuple(std::forward<TArgs>(args)...));
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
            return std::static_pointer_cast<TTarget>(_constructorNew(std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @return
        template<typename... TArgs>
        requires (sizeof...(TArgs) == 0 || Concepts::ValidConstructorArgs<TArgs...>)
        std::shared_ptr<void> New(TArgs&&... args) const
        {
            return _constructorNew(std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        TTarget Create(const std::vector<std::any>& args) const
        {
            return std::any_cast<TTarget>(_fromAny(args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::vector<std::any>& args) const
        {
            return std::static_pointer_cast<TTarget>(_fromAnyNew(args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        TTarget Create(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::any_cast<TTarget>(_fromPtr(args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args 
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::static_pointer_cast<TTarget>(_fromPtrNew(args));
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

        /// @brief
        /// @tparam TArgs
        /// @param name
        /// @return
        template<typename TConstructable, typename... TArgs>
        static std::shared_ptr<MemberInfo> Create(std::string&& name) noexcept
        {
            return std::make_shared<ConstructorInfo>(std::move(name), Helper<TConstructable, TArgs...>(), ToTypeIndexes<TArgs...>());
        }
    };
}

#define CONSTRUCTOR(...) \
ExtendedCpp::Reflection::ConstructorInfo::Create<ThisClassType __VA_OPT__(,) __VA_ARGS__>(std::string(typeid(ThisClassType).name()))

#endif