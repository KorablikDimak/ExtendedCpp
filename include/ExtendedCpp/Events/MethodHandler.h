#ifndef Events_MethodHandler_H
#define Events_MethodHandler_H

#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TObject 
    /// @tparam ...TParams 
    template<typename TObject, typename ...TParams>
    class MethodHandler final : public IEventHandler<TParams...>
    {
    private:
        TObject* _object;
        void (TObject::*_method)(TParams... params);

    public:
        /// @brief 
        /// @param object 
        /// @param method 
        MethodHandler(TObject* object, void(TObject::*method)(TParams...)) noexcept
        {
            _object = object;
            _method = method;
        }

        /// @brief 
        ~MethodHandler() override = default;

        /// @brief 
        /// @param ...params 
        void Call(TParams... params) const override
        {
            if (_object != nullptr)
                (_object->*_method)(std::forward<TParams>(params)...);
        }

        /// @brief 
        /// @param object 
        /// @param method 
        /// @return 
        bool IsEquals(TObject* object, void(TObject::*method)(TParams...)) const noexcept
        {
            return _object == object && _method == method;
        }

    protected:
        /// @brief 
        /// @param other 
        /// @return 
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* methodHandler = dynamic_cast<const MethodHandler*>(&other);
            if (!methodHandler)
                return false;
            return methodHandler->IsEquals(_object, _method);
        }
    };

    /// @brief 
    /// @tparam TObject 
    /// @tparam ...TParams 
    /// @param object 
    /// @param method 
    /// @return 
    template<typename TObject, typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateMethodHandler(TObject* object, void(TObject::*method)(TParams...)) noexcept
    {
        return std::make_shared<MethodHandler<TObject, TParams...>>(object, method);
    }
}

#define METHOD_HANDLER(object, method) \
ExtendedCpp::Events::CreateMethodHandler(object, method)

#define THIS_METHOD_HANDLER(method) \
METHOD_HANDLER(this, method)

#endif