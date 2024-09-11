module;

#include <any>
#include <stdexcept>
#include <vector>
#include <memory>
#include <typeindex>

export module ExtendedCpp.Reflection.ConstructorInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.TypeTraits;
import ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
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
            [[nodiscard]]
            decltype(auto) Get(const std::vector<std::any>& args, std::index_sequence<Index...>) const noexcept
            {
                return std::any_cast<typename std::tuple_element<Index, TuppleArgs>::type...>(args[Index]...);
            }

            template<std::size_t... Index>
            [[nodiscard]]
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
        std::any CreateFromAny(const std::vector<std::any>& args) const
        {
            return _fromAny(_constructorHelper, args);
        }

        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::any>& args) const
        {
            return _fromAnyNew(_constructorHelper, args);
        }

        [[nodiscard]]
        std::any CreateFromAny(const std::vector<std::shared_ptr<void>>& args) const
        {
            return _fromAnyPtr(_constructorHelper, args);
        }

        [[nodiscard]]
        std::shared_ptr<void> NewFromAny(const std::vector<std::shared_ptr<void>>& args) const
        {
            return _fromAnyPtrNew(_constructorHelper, args);
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Constructor;
        }

        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
    };
}