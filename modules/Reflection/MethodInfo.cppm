module;

#include <any>
#include <stdexcept>
#include <vector>
#include <typeindex>

export module ExtendedCpp.Reflection.MethodInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.MemberType;

namespace ExtendedCpp::Reflection
{
    class MethodInfo final : public MemberInfo
    {
    private:
        std::any _methodHelper;
        std::any (*_method)(const std::any& helper, std::any&& object, std::any&& args);
        std::vector<std::type_index> _parameters{};

    public:
        template<typename TObject, typename TMethod, typename TReturnType>
        struct Helper final
        {
        private:
            TMethod _method;

        public:
            using ReturnType = TReturnType;

            explicit Helper(TMethod&& method) noexcept :
                    _method(std::forward<TMethod>(method)) {}

            TReturnType Invoke(std::any&& object, std::any&& args) const
            {
                return _method(std::any_cast<TObject*>(object), args);
            }
        };

        template<typename THelper>
        MethodInfo(const std::string& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
                _methodHelper(std::forward<THelper>(methodHelper)),
                _method([](const std::any& helper, std::any&& object, std::any&& args)
                        {
                            if constexpr (std::same_as<void, typename THelper::ReturnType>) return std::any();
                            else return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args)));
                        }),
                _parameters(std::move(parameters)),
                MemberInfo(methodName) {}

        template<typename THelper>
        MethodInfo(std::string&& methodName, THelper&& methodHelper, std::vector<std::type_index>&& parameters) noexcept :
                _methodHelper(std::forward<THelper>(methodHelper)),
                _method([](const std::any& helper, std::any&& object, std::any&& args)
                        {
                            if constexpr (std::same_as<void, typename THelper::ReturnType>) return std::any();
                            else return std::any(std::any_cast<const THelper&>(helper).Invoke(std::move(object), std::move(args)));
                        }),
                _parameters(std::move(parameters)),
                MemberInfo(std::move(methodName)) {}

        ~MethodInfo() override = default;

        template<typename TResult, typename TObject, typename... TArgs>
        TResult Invoke(TObject* object, TArgs&&... args) const
        {
            if (object == nullptr) return {};
            return std::any_cast<TResult>(_method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...)));
        }

        template<typename TObject, typename... TArgs>
        std::any Invoke(TObject* object, TArgs&&... args) const
        {
            if (object == nullptr) return {};
            return _method(_methodHelper, object, std::make_tuple(std::forward<TArgs>(args)...));
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Method;
        }

        [[nodiscard]]
        std::vector<std::type_index> Parameters() const noexcept
        {
            return _parameters;
        }
    };
}