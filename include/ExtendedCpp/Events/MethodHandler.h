#ifndef Events_MethodHandler_H
#define Events_MethodHandler_H

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TObject 
    /// @tparam TParams
    template<typename TObject, typename ...TParams>
    class MethodHandler final : public IEventHandler<TParams...>
    {
    private:
        mutable TObject* _object;
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
        /// @param params
        void Call(TParams&&... params) const override
        {
            if (_object != nullptr)
                (_object->*_method)(std::forward<TParams>(params)...);
        }
    };
}

#endif