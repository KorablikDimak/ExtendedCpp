#ifndef Events_MethodHandler_H
#define Events_MethodHandler_H

#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

namespace ExtendedCpp::Events
{
    template<typename TObject, typename ...TParams>
    class MethodHandler final : public IEventHandler<TParams...>
    {
    private:
        TObject* _object;
        void (TObject::*_method)(TParams... params);

    public:
        MethodHandler(TObject* object, void(TObject::*method)(TParams...)) noexcept
        {
            _object = object;
            _method = method;
        }

        ~MethodHandler() override = default;

        void Call(TParams... params) const override
        {
            if (_object != nullptr)
                (_object->*_method)(std::forward<TParams>(params)...);
        }

        bool IsEquals(TObject* object, void(TObject::*method)(TParams...)) const noexcept
        {
            return _object == object && _method == method;
        }

    protected:
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* methodHandler = static_cast<const MethodHandler*>(&other);
            return methodHandler->IsEquals(_object, _method);
        }
    };

    template<typename TObject, typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateMethodHandler(TObject* object, void(TObject::*method)(TParams...)) noexcept
    {
        return std::shared_ptr<IEventHandler<TParams...>>(new MethodHandler<TObject, TParams...>(object, method));
    }
}

#define METHOD_HANDLER(object, method) \
ExtendedCpp::Events::CreateMethodHandler(object, method)

#define THIS_METHOD_HANDLER(method) \
METHOD_HANDLER(this, method)

#endif