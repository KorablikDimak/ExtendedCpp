#ifndef Reflection_ConstructorInfo_H
#define Reflection_ConstructorInfo_H

#include <any>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    #define CONSTRUCTOR(...) \
    std::make_shared<ConstructorInfo>(std::string(typeid(*this).name()), \
    ConstructorInfo::Helper<std::remove_pointer_t<decltype(this)> __VA_OPT__(,) __VA_ARGS__>())

    class ConstructorInfo final : public MemberInfo
    {
    private:
        std::any _constructorHelper;
        std::any (*_constructor)(std::any&, std::any args);

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
        };

        template<typename THelper>
        ConstructorInfo(const std::string& constructorName, THelper constructorHelper) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            MemberInfo(constructorName) {}

        template<typename THelper>
        ConstructorInfo(std::string&& constructorName, THelper constructorHelper) noexcept :
            _constructorHelper(constructorHelper),
            _constructor([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Create(args)); }),
            MemberInfo(std::move(constructorName)) {}

        ~ConstructorInfo() override = default;

        template<typename... TArgs>
        auto Create(TArgs... args){ return _constructor(_constructorHelper, std::make_tuple(args...)); }

        [[nodiscard]]
        inline Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Constructor;
        }
    };
}

#endif