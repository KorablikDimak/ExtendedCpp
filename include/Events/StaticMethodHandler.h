#ifndef Events_StaticMethodHandler_H
#define Events_StaticMethodHandler_H

#include <memory>

#include <Events/IEventHandler.h>

namespace Events
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

        void Call(TParams... params) override
        {
            (*_method)(params...);
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
        return std::shared_ptr<IEventHandler<TParams...>>(new StaticMethodHandler<TParams...>(method));
    }
}

#define STATIC_METHOD_HANDLER(method) \
Events::CreateStaticMethodHandler(method)

#endif