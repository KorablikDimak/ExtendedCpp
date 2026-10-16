#ifndef EVENTS_STATIC_METHOD_HANDLER_H
#define EVENTS_STATIC_METHOD_HANDLER_H

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TParams
    template<typename ...TParams>
    class FunctionHandler final : public IEventHandler<TParams...>
    {
    private:
        void (*_function)(TParams... params);

    public:
        /// @brief 
        /// @param function
        explicit FunctionHandler(void(*function)(TParams...)) noexcept :
            _function(function) {}

        /// @brief
        ~FunctionHandler() override = default;

        /// @brief 
        /// @param params
        void Call(TParams&&... params) const override
        {
            (*_function)(std::forward<TParams>(params)...);
        }
    };
}

#endif