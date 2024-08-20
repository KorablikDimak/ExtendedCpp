#ifndef Reflection_ConstructorInfo_H
#define Reflection_ConstructorInfo_H

#include <any>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    #define CONSTRUCTOR(...) \
    []()->std::shared_ptr<MemberInfo> \
    { \
        return std::make_shared<ConstructorInfo>(std::string(typeid(ThisClassType).name()), \
        ConstructorInfo::Helper<ThisClassType __VA_OPT__(,) __VA_ARGS__>(), \
        ToTypeIndexes<__VA_ARGS__>()); \
    }()

    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any _constructorHelper;
        std::any (*_constructor)(std::any&, std::any args);
        std::any (*_constructorNew)(std::any&, std::any args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TClass, typename... TArgs>
        struct Helper final
        {
            using TuppleArgs = std::tuple<TArgs...>;

            TClass Create(std::any& args)
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return TClass();
                else
                    return std::apply([](TArgs... args)
                        { return TClass(args...); }, std::any_cast<TuppleArgs>(args));
            }

            TClass* New(std::any& args)
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                    return new TClass();
                else
                    return std::apply([](TArgs... args)
                        { return new TClass(args...); }, std::any_cast<TuppleArgs>(args));
            }
        };

        template<typename THelper>
        ConstructorInfo(const std::string& constructorName, THelper constructorHelper, std::vector<std::type_index> parameters) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            _constructorNew([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).New(args)); }),
            _parameters(parameters),
            MemberInfo(constructorName) {}

        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper constructorHelper, std::vector<std::type_index> parameters) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            _constructorNew([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).New(args)); }),
            _parameters(parameters),
            MemberInfo(std::move(constructorName)) {}

        ~ConstructorInfo() override = default;

        template<typename... TArgs>
        auto Create(TArgs... args){ return _constructor(_constructorHelper, std::make_tuple(args...)); }

        template<typename... TArgs>
        auto New(TArgs... args){ return _constructorNew(_constructorHelper, std::make_tuple(args...)); }

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

#endif