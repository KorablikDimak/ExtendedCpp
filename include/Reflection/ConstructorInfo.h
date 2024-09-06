#ifndef Reflection_ConstructorInfo_H
#define Reflection_ConstructorInfo_H

#include <any>

#include <Reflection/MemberInfo.h>
#include <Reflection/TypeTraits.h>

namespace Reflection
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
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return TClass();
                else
                    return std::apply([](TArgs... args)
                        { return TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }

            TClass* New(const std::any& args) const
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return new TClass();
                else
                    return std::apply([](TArgs&&... args)
                        { return new TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }

            template<std::size_t... Index>
            decltype(auto) Get(const std::vector<std::any>& args, std::index_sequence<Index...>) const noexcept
            {
                return std::any_cast<typename std::tuple_element<Index, TuppleArgs>::type...>(args[Index]...);
            }

            template<std::size_t... Index>
            decltype(auto) Get(const std::vector<std::shared_ptr<void>>& args, std::index_sequence<Index...>) const noexcept
            {
                return std::static_pointer_cast<typename std::tuple_element<Index, TuppleArgs>::type::element_type...>(args[Index]...);
            }

            TClass CreateFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                {
                    return TClass();
                }
                else
                {
                    if (args.size() != Size) throw std::range_error("Incorrect args size in constructor");
                    return TClass(Get(args, std::make_index_sequence<Size>()));
                }
            }

            TClass CreateFromAnyPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                {
                    return TClass();
                }
                else if constexpr (!IsSharedPtr<TArgs...>::value)
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != Size) throw std::range_error("Incorrect args size in constructor");
                    return TClass(Get(args, std::make_index_sequence<Size>()));
                }
            }

            TClass* NewFromAny(const std::vector<std::any>& args) const
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                {
                    return new TClass();
                }
                else
                {
                    if (args.size() != Size) throw std::range_error("Incorrect args size in constructor");
                    return new TClass(Get(args, std::make_index_sequence<Size>()));
                }
            }

            TClass* NewFromAnyPtr(const std::vector<std::shared_ptr<void>>& args) const
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                {
                    return new TClass();
                }
                else if constexpr (!IsSharedPtr<TArgs...>::value)
                {
                    throw std::runtime_error("Class can not construct from shared_ptr");
                }
                else
                {
                    if (args.size() != Size) throw std::range_error("Incorrect args size in constructor");
                    return new TClass(Get(args, std::make_index_sequence<Size>()));
                }
            }
        };

        template<typename THelper>
        ConstructorInfo(const std::string& constructorName, THelper&& constructorHelper, const std::vector<std::type_index>& parameters) noexcept :
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
            _parameters(parameters),
            MemberInfo(constructorName) {}

        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper&& constructorHelper, std::vector<std::type_index>&& parameters) noexcept :
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
            _parameters(std::move(parameters)),
            MemberInfo(std::move(constructorName)) {}

        ~ConstructorInfo() override = default;

        template<typename... TArgs>
        std::any Create(TArgs&&... args) const
        {
            return _constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        template<typename... TArgs>
        std::shared_ptr<void> New(TArgs&&... args) const
        {
            return _constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        std::any CreateFromAny(const std::vector<std::any>& args) const;
        std::any CreateFromAny(std::vector<std::any>&& args) const;
        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::any>& args) const;
        std::shared_ptr<void> NewFromAny(std::vector<std::any>&& args) const;
        [[nodiscard]]
        std::any CreateFromAny(const std::vector<std::shared_ptr<void>>& args) const;
        std::any CreateFromAny(std::vector<std::shared_ptr<void>>&& args) const;
        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::shared_ptr<void>>& args) const;
        std::shared_ptr<void> NewFromAny(std::vector<std::shared_ptr<void>>&& args) const;

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept;
    };
}

#define CONSTRUCTOR(...) \
[]()->std::shared_ptr<Reflection::MemberInfo> \
{ \
    return std::make_shared<Reflection::ConstructorInfo>(std::string(typeid(ThisClassType).name()), \
        Reflection::ConstructorInfo::Helper<ThisClassType __VA_OPT__(,) __VA_ARGS__>(), \
        Reflection::ToTypeIndexes<__VA_ARGS__>()); \
}()

#endif