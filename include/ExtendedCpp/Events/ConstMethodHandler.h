#ifndef Events_ConstMethodHandler_H
#define Events_ConstMethodHandler_H

#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

namespace ExtendedCpp::Events
{
    template<typename TObject, typename ...TParams>
    class ConstMethodHandler final : public IEventHandler<TParams...>
    {
    private:
        TObject* _object;
        void (TObject::*_method)(TParams... params) const;

    public:
        ConstMethodHandler(TObject* object, void(TObject::*method)(TParams...) const) noexcept
        {
            _object = object;
            _method = method;
        }

        ~ConstMethodHandler() override = default;

        void Call(TParams... params) const override
        {
            if (_object != nullptr)
                (_object->*_method)(std::forward<TParams>(params)...);
        }

        bool IsEquals(TObject* object, void(TObject::*method)(TParams...) const) const noexcept
        {
            return _object == object && _method == method;
        }

    protected:
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* methodHandler = dynamic_cast<const ConstMethodHandler<TObject, TParams...>*>(&other);
            if (!methodHandler)
                return false;
            return methodHandler->IsEquals(_object, _method);
        }
    };

    template<typename TObject, typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateConstMethodHandler(TObject* object, void(TObject::*method)(TParams...) const) noexcept
    {
        return std::make_shared<ConstMethodHandler<TObject, TParams...>>(object, method);
    }
}

#define CONST_METHOD_HANDLER(object, method) \
ExtendedCpp::Events::CreateConstMethodHandler(object, method)

#define THIS_CONST_METHOD_HANDLER(method) \
CONST_METHOD_HANDLER(this, method)

#endif