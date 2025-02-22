#ifndef Events_StaticMethodHandler_H
#define Events_StaticMethodHandler_H

#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam ...TParams 
    template<typename ...TParams>
    class StaticMethodHandler final : public IEventHandler<TParams...>
    {
    private:
        void (*_method)(TParams... params);

    public:
        /// @brief 
        /// @param method 
        explicit StaticMethodHandler(void(*method)(TParams...)) noexcept
        {
            _method = method;
        }

        /// @brief 
        ~StaticMethodHandler() override = default;

        /// @brief 
        /// @param ...params 
        void Call(TParams... params) const override
        {
            (*_method)(std::forward<TParams>(params)...);
        }

        /// @brief 
        /// @param method 
        /// @return 
        bool IsEquals(void(*method)(TParams...)) const noexcept
        {
            return _method == method;
        }

    protected:
        /// @brief 
        /// @param other 
        /// @return 
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* staticMethodHandler = static_cast<const StaticMethodHandler*>(&other);
            return staticMethodHandler->IsEquals(_method);
        }
    };

    /// @brief 
    /// @tparam ...TParams 
    /// @param method 
    /// @return 
    template<typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateStaticMethodHandler(void(*method)(TParams...)) noexcept
    {
        return std::make_shared<StaticMethodHandler<TParams...>>(method);
    }
}

#define STATIC_METHOD_HANDLER(method) \
ExtendedCpp::Events::CreateStaticMethodHandler(method)

#endif