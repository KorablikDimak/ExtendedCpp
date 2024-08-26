#ifndef Reflection_ConstructorInfo_H
#define Reflection_ConstructorInfo_H

#include <any>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any _constructorHelper;
        std::any (*_constructor)(std::any&, std::any args);
        std::shared_ptr<void> (*_constructorNew)(std::any&, std::any args){};
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TClass, typename... TArgs>
        struct Helper final
        {
            using TuppleArgs = std::tuple<TArgs...>;

            TClass Create(const std::any& args)
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return TClass();
                else
                    return std::apply([](TArgs... args)
                        { return TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }

            TClass* New(const std::any& args)
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return new TClass();
                else
                    return std::apply([](TArgs&&... args)
                        { return new TClass(std::forward<TArgs>(args)...); }, std::any_cast<TuppleArgs>(args));
            }
        };

        template<typename THelper>
        ConstructorInfo(const std::string& constructorName, THelper constructorHelper, std::vector<std::type_index> parameters) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            _constructorNew([](std::any& helper, std::any args){ return std::shared_ptr<void>(std::any_cast<THelper&>(helper).New(args)); }),
            _parameters(parameters),
            MemberInfo(constructorName) {}

        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper constructorHelper, std::vector<std::type_index> parameters) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            _constructorNew([](std::any& helper, std::any args){ return std::shared_ptr<void>(std::any_cast<THelper&>(helper).New(args)); }),
            _parameters(parameters),
            MemberInfo(std::move(constructorName)) {}

        ~ConstructorInfo() override = default;

        template<typename... TArgs>
        auto Create(TArgs... args)
        {
            return _constructor(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        template<typename... TArgs>
        auto New(TArgs... args)
        {
            return _constructorNew(_constructorHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        inline Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Constructor;
        }

        [[nodiscard]]
        inline std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
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