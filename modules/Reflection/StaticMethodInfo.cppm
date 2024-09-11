module;

#include <any>
#include <stdexcept>
#include <utility>
#include <vector>
#include <typeindex>

export module ExtendedCpp.Reflection.StaticMethodInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
{
    class StaticMethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(const std::any& helper, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TMethod, typename TReturnType>
        struct Helper final
        {
        private:
            TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TMethod&& method) noexcept :
                    _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& args) const
            {
                return _method(std::move(args));
            }
        };

        template<typename THelper>
        StaticMethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
                _methodHelper(std::forward<THelper>(methodHelper)),
                _method([](const std::any& helper, std::any&& args)
                        {
                            if constexpr (std::same_as<void, typename THelper::ReturnType>) return std::any();
                            return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(args)));
                        }),
                _parameters(std::move(parameters)),
                MemberInfo(methodName) {}

        template<typename THelper>
        StaticMethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
                _methodHelper(std::forward<THelper>(methodHelper)),
                _method([](const std::any& helper, std::any&& args)
                        {
                            if constexpr (std::same_as<void, typename THelper::ReturnType>) return std::any();
                            return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(args)));
                        }),
                _parameters(std::move(parameters)),
                MemberInfo(std::move(methodName)) {}

        ~StaticMethodInfo() override = default;

        template<typename TResult, typename... TArgs>
        TResult Invoke(TArgs&&... args) const
        {
            return std::any_cast<TResult>(_method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename... TArgs>
        std::any Invoke(TArgs&&... args) const
        {
            return _method(_methodHelper, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::StaticMethod;
        }

        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
    };
}