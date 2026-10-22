#ifndef REFLECTION_CONSTRUCTOR_INFO_H
#define REFLECTION_CONSTRUCTOR_INFO_H

#include <any>
#include <span>
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
        const std::vector<std::type_index> _parameters;

        std::any (*const _create)(std::any&& args);
        std::any (*const _createFromAny)(std::span<const std::any> args){};
        std::any (*const _createFromPtr)(std::span<const std::shared_ptr<void>> args){};

        std::shared_ptr<void> (*const _new)(std::any&& args){};
        std::shared_ptr<void> (*const _newFromAny)(std::span<const std::any> args){};
        std::shared_ptr<void> (*const _newFromPtr)(std::span<const std::shared_ptr<void>> args){};

        template<typename TClass, typename... TArgs>
        struct Helper final
        {
        private:
            using TupleArgs = std::tuple<TArgs...>;
            static constexpr std::size_t SIZE = sizeof...(TArgs);

        public:
            static TClass Create(std::any&& args)
            {
                return std::apply([](TArgs&&... argsInner)
                    { return TClass(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            static std::shared_ptr<void> New(std::any&& args)
            {
                return std::apply([](TArgs&&... argsInner)
                    { return std::make_shared<TClass>(std::forward<TArgs>(argsInner)...); }, std::any_cast<TupleArgs>(std::move(args)));
            }

            static TClass Create(const std::span<const std::any> args)
            {
                if (args.size() != SIZE)
                    throw std::range_error("Incorrect args size in constructor");

                return [args]<std::size_t... Index>(std::index_sequence<Index...>)
                {
                    return TClass(std::any_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                }(std::make_index_sequence<SIZE>());
            }

            static std::shared_ptr<void> New(const std::span<const std::any> args)
            {
                if (args.size() != SIZE)
                    throw std::range_error("Incorrect args size in constructor");

                return [args]<std::size_t... Index>(std::index_sequence<Index...>)
                {
                    return std::make_shared<TClass>(std::any_cast<std::tuple_element_t<Index, TupleArgs>>(args[Index])...);
                }(std::make_index_sequence<SIZE>());
            }

            static TClass Create(const std::span<const std::shared_ptr<void>> args)
            {
                if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                    throw std::domain_error("Class can not construct from shared_ptr");
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)
                    {
                        return TClass(std::static_pointer_cast<typename std::tuple_element_t<Index, TupleArgs>::element_type>(args[Index])...);
                    }(std::make_index_sequence<SIZE>());
                }
            }

            static std::shared_ptr<void> New(const std::span<const std::shared_ptr<void>> args)
            {
                if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                    throw std::domain_error("Class can not construct from shared_ptr");
                else
                {
                    if (args.size() != SIZE)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)
                    {
                        return std::make_shared<TClass>(std::static_pointer_cast<typename std::tuple_element_t<Index, TupleArgs>::element_type>(args[Index])...);
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
            _parameters(std::move(parameters)),

            _create([](std::any&& args)
                { return std::any(std::remove_cvref_t<THelper>::Create(std::move(args))); }),
            _createFromAny([](const std::span<const std::any> args)
                { return std::any(std::remove_cvref_t<THelper>::Create(args)); }),
            _createFromPtr([](const std::span<const std::shared_ptr<void>> args)
                { return std::any(std::remove_cvref_t<THelper>::Create(args)); }),

            _new([](std::any&& args)
                { return std::remove_cvref_t<THelper>::New(std::move(args)); }),
            _newFromAny([](const std::span<const std::any> args)
                { return std::remove_cvref_t<THelper>::New(args); }),
            _newFromPtr([](const std::span<const std::shared_ptr<void>> args)
                { return std::remove_cvref_t<THelper>::New(args); }) {}

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(pop)
#endif

        /// @brief
        ~ConstructorInfo() noexcept override = default;

        /// @brief 
        /// @tparam TTarget 
        /// @tparam TArgs
        /// @param args
        /// @throw std::bad_cast
        /// @return
        template<typename TTarget, Concepts::ValidArgs... TArgs>
        std::shared_ptr<MemberInfo> Create(TArgs&&... args) const
        {
            return std::any_cast<TTarget>(_create(std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @throw std::bad_cast
        /// @return
        template<Concepts::ValidArgs... TArgs>
        std::any Create(TArgs&&... args) const
        {
            return _create(std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief
        /// @tparam TTarget
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @return
        template<typename TTarget>
        TTarget Create(const std::span<const std::any> args) const
        {
            return std::any_cast<TTarget>(_createFromAny(args));
        }

        /// @brief
        /// @tparam TTarget
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @throw std::domain_error
        /// @return
        template<typename TTarget>
        TTarget Create(const std::span<const std::shared_ptr<void>> args) const
        {
            return std::any_cast<TTarget>(_createFromPtr(args));
        }

        /// @brief
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @return
        [[nodiscard]]
        std::any Create(std::span<const std::any> args) const;

        /// @brief
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @throw std::domain_error
        /// @return
        [[nodiscard]]
        std::any Create(std::span<const std::shared_ptr<void>> args) const;

        /// @brief 
        /// @tparam TTarget 
        /// @tparam TArgs
        /// @param args
        /// @throw std::bad_cast
        /// @return
        template<typename TTarget, Concepts::ValidArgs... TArgs>
        std::shared_ptr<TTarget> New(TArgs&&... args) const
        {
            return std::static_pointer_cast<TTarget>(_new(std::make_tuple(std::forward<TArgs>(args)...)));
        }

        /// @brief 
        /// @tparam TArgs
        /// @param args
        /// @throw std::bad_cast
        /// @return
        template<Concepts::ValidArgs... TArgs>
        std::shared_ptr<void> New(TArgs&&... args) const
        {
            return _new(std::make_tuple(std::forward<TArgs>(args)...));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::span<const std::any> args) const
        {
            return std::static_pointer_cast<TTarget>(_newFromAny(args));
        }

        /// @brief 
        /// @tparam TTarget 
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @throw std::domain_error
        /// @return
        template<typename TTarget>
        std::shared_ptr<TTarget> New(const std::span<const std::shared_ptr<void>> args) const
        {
            return std::static_pointer_cast<TTarget>(_newFromPtr(args));
        }

        /// @brief 
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @return
        [[nodiscard]]
        std::shared_ptr<void> New(std::span<const std::any> args) const;

        /// @brief 
        /// @param args
        /// @throw std::bad_cast
        /// @throw std::range_error
        /// @throw std::domain_error
        /// @return
        [[nodiscard]]
        std::shared_ptr<void> New(std::span<const std::shared_ptr<void>> args) const;

        /// @brief 
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::span<const std::type_index> Parameters() const noexcept;

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