#ifndef Reflection_ConstructorInfo_H
#define Reflection_ConstructorInfo_H

#include <any>
#include <stdexcept>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>
#include <ExtendedCpp/Reflection/Concepts.h>

namespace ExtendedCpp::Reflection
{
    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any _constructorHelper;

        std::any (*_constructor)(const std::any& helper, std::any args);
        std::shared_ptr<void> (*_constructorNew)(const std::any& helper, std::any args){};

        std::any (*_fromAny)(const std::any& helper, const std::vector<std::any>& args){};
        std::shared_ptr<void> (*_fromAnyNew)(const std::any& helper, const std::vector<std::any>& args){};

        std::any (*_fromAnyPtr)(const std::any& helper, const std::vector<std::shared_ptr<void>>& args){};
        std::shared_ptr<void> (*_fromAnyPtrNew)(const std::any& helper, const std::vector<std::shared_ptr<void>>& args){};

        std::vector<std::type_index> _parameters{};

    public:
        template<typename TClass, typename... TArgs>
        struct Helper final
        {
            using TuppleArgs = std::tuple<TArgs...>;
            static constexpr std::size_t Size = std::tuple_size_v<TuppleArgs>;

            TClass Create(const std::any& args) const
            {
                if constexpr (!Size)
                    return TClass();
                else
                    return std::apply([](TArgs&&... args)
                        { return TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }

            TClass* New(const std::any& args) const
            {
                if constexpr (!Size)
                    return new TClass();
                else
                    return std::apply([](TArgs&&... args)
                        { return new TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }

            TClass CreateFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (!Size)
                {
                    return TClass();
                }
                else
                {
                    if (args.size() != Size)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass
                    {
                        return TClass(std::any_cast<typename std::tuple_element<Index, TuppleArgs>::type>(args[Index])...);
                    }(std::make_index_sequence<Size>());
                }
            }

            TClass CreateFromAnyPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (!Size)
                {
                    return TClass();
                }
                else if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != Size)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass
                    {
                        return TClass(std::static_pointer_cast<typename std::tuple_element<Index, TuppleArgs>::type::element_type>(args[Index])...);
                    }(std::make_index_sequence<Size>());
                }
            }

            TClass* NewFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (!Size)
                {
                    return new TClass();
                }
                else
                {
                    if (args.size() != Size)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass*
                    {
                        return new TClass(std::any_cast<typename std::tuple_element<Index, TuppleArgs>::type>(args[Index])...);
                    }(std::make_index_sequence<Size>());
                }
            }

            TClass* NewFromAnyPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (!Size)
                {
                    return new TClass();
                }
                else if constexpr (!(Concepts::IsSharedPtr<TArgs> && ...))
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != Size)
                        throw std::range_error("Incorrect args size in constructor");

                    return [args]<std::size_t... Index>(std::index_sequence<Index...>)->TClass*
                    {
                        return new TClass(std::static_pointer_cast<typename std::tuple_element<Index, TuppleArgs>::type::element_type>(args[Index])...);
                    }(std::make_index_sequence<Size>());
                }
            }
        };

        template<typename THelper>
        ConstructorInfo(const std::string& constructorName, THelper&& constructorHelper, const std::vector<std::type_index>& parameters) noexcept :
            MemberInfo(constructorName),
            _constructorHelper(std::forward<THelper>(constructorHelper)),
            _constructor([](const std::any& helper, std::any args)
                { return std::any(std::any_cast<const THelper&>(helper).Create(args)); }),
            _constructorNew([](const std::any& helper, std::any args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).New(args)); }),
            _fromAny([](const std::any& helper, const std::vector<std::any>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromAny(args)); }),
            _fromAnyNew([](const std::any& helper, const std::vector<std::any>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromAny(args)); }),
            _fromAnyPtr([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromAnyPtr(args)); }),
            _fromAnyPtrNew([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromAnyPtr(args)); }),
            _parameters(parameters) {}

        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper&& constructorHelper, std::vector<std::type_index>&& parameters) noexcept :
            MemberInfo(std::move(constructorName)),
            _constructorHelper(std::forward<THelper>(constructorHelper)),
            _constructor([](const std::any& helper, std::any args)
                { return std::any(std::any_cast<const THelper&>(helper).Create(args)); }),
            _constructorNew([](const std::any& helper, std::any args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).New(args)); }),
            _fromAny([](const std::any& helper, const std::vector<std::any>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromAny(args)); }),
            _fromAnyNew([](const std::any& helper, const std::vector<std::any>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromAny(args)); }),
            _fromAnyPtr([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::any(std::any_cast<const THelper&>(helper).CreateFromAnyPtr(args)); }),
            _fromAnyPtrNew([](const std::any& helper, const std::vector<std::shared_ptr<void>>& args)
                { return std::shared_ptr<void>(std::any_cast<const THelper&>(helper).NewFromAnyPtr(args)); }),
            _parameters(std::move(parameters)) {}

        ~ConstructorInfo() override = default;

        template<typename TTarget, typename... TArgs>
        TTarget Create(TArgs&&... args) const
        {
            return std::any_cast<TTarget>
                    (_constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename... TArgs>
        std::any Create(TArgs&&... args) const
        {
            return _constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        template<typename TTarget, typename... TArgs>
        std::shared_ptr<TTarget> New(TArgs&&... args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename... TArgs>
        std::shared_ptr<void> New(TArgs&&... args) const
        {
            return _constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        template<typename TTarget>
        TTarget CreateFromAny(const std::vector<std::any>& args) const
        {
            return std::any_cast<TTarget>
                    (_fromAny(_constructorHelper, args));
        }

        template<typename TTarget>
        std::shared_ptr<TTarget> NewFromAny(const std::vector<std::any>& args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_fromAnyNew(_constructorHelper, args));
        }

        template<typename TTarget>
        TTarget CreateFromAny(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::any_cast<TTarget>
                    (_fromAnyPtr(_constructorHelper, args));
        }

        template<typename TTarget>
        std::shared_ptr<TTarget> NewFromAny(const std::vector<std::shared_ptr<void>>& args) const
        {
            return std::static_pointer_cast<TTarget>
                    (_fromAnyPtrNew(_constructorHelper, args));
        }

        [[nodiscard]]
        std::any CreateFromAny(const std::vector<std::any>& args) const;
        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::any>& args) const;
        [[nodiscard]]
        std::any CreateFromAny(const std::vector<std::shared_ptr<void>>& args) const;
        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::shared_ptr<void>>& args) const;

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
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