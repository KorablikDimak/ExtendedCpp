#ifndef Events_StaticMethodHandler_H
#define Events_StaticMethodHandler_H

#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

namespace ExtendedCpp::Events
{
    template<typename ...TParams>
    class StaticMethodHandler final : public IEventHandler<TParams...>
    {
    private:
        void (*_method)(TParams... params);

    public:
        explicit StaticMethodHandler(void(*method)(TParams...)) noexcept
        {
            _method = method;
        }

        ~StaticMethodHandler() override = default;

        void Call(TParams... params) const override
        {
            (*_method)(std::forward<TParams>(params)...);
        }

        bool IsEquals(void(*method)(TParams...)) const noexcept
        {
            return _method == method;
        }

    protected:
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* staticMethodHandler = static_cast<const StaticMethodHandler*>(&other);
            return staticMethodHandler->IsEquals(_method);
        }
    };

    template<typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateStaticMethodHandler(void(*method)(TParams...)) noexcept
    {
        return std::make_shared<StaticMethodHandler<TParams...>>(method);
    }
}

#define STATIC_METHOD_HANDLER(method) \
ExtendedCpp::Events::CreateStaticMethodHandler(method)

#endif