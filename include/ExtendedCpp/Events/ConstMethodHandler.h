#ifndef Events_ConstMethodHandler_H
#define Events_ConstMethodHandler_H

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TObject 
    /// @tparam TParams
    template<typename TObject, typename ...TParams>
    class ConstMethodHandler final : public IEventHandler<TParams...>
    {
    private:
        TObject* _object;
        void (TObject::*_method)(TParams... params) const;

    public:
        /// @brief 
        /// @param object 
        /// @param method 
        ConstMethodHandler(TObject* object, void(TObject::*method)(TParams...) const) noexcept
        {
            _object = object;
            _method = method;
        }

        /// @brief 
        ~ConstMethodHandler() override = default;

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