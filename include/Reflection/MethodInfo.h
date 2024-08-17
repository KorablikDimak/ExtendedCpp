#ifndef Reflection_MethodInfo_H
#define Reflection_MethodInfo_H

#include <functional>
#include <any>

#include <Reflection/MemberInfo.h>
#include <Reflection/MethodTraits.h>

namespace Reflection
{
    #define METHOD(methodName, ...) \
    std::make_shared<MethodInfo>(#methodName, \
    MethodInfo::Helper<std::remove_pointer_t<decltype(this)>, \
    decltype(&std::remove_pointer_t<decltype(this)>::methodName) __VA_OPT__(,) __VA_ARGS__> \
    (this, &std::remove_pointer_t<decltype(this)>::methodName))

    class MethodInfo : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(std::any&, std::any args);

    public:
        template<typename TObject, typename TMethod, typename... TArgs>
        struct Helper
        {
            using TuppleArgs = std::tuple<TArgs...>;
            using ReturnType = MethodTraits<TObject, TMethod(TArgs...)>::ReturnType;

            TObject* _object;
            TMethod _method;

            Helper(TObject* object, TMethod&& method)
            {
                _object = object;
                _method = method;
            }

            ReturnType Invoke(std::any& args)
            {
                if constexpr (std::tuple_size_v<TuppleArgs> == 0)
                {
                    if (_object != nullptr)
                        return (_object->*_method)();
                    else
                        throw std::domain_error("object is null");
                }
                else
                {
                    if (_object != nullptr)
                        return std::apply([this](TArgs... args)
                            { return (_object->*_method)(args...); }, std::any_cast<TuppleArgs>(args));
                    else
                        throw std::domain_error("object is null");
                }
            }
        };

        template<typename THelper>
        MethodInfo(std::string&& methodName, THelper methodHelper) noexcept :
            _methodHelper(methodHelper),
            _method([](std::any& helper, std::any args){ return std::any(std::any_cast<THelper&>(helper).Invoke(args)); }),
            MemberInfo(std::move(methodName)) {}

        ~MethodInfo() override = default;

        template<typename... TArgs>
        auto Invoke(TArgs... args){ return _method(_methodHelper, std::make_tuple(args...)); }

        [[nodiscard]]
        inline Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Method;
        }
    };
}

#endif