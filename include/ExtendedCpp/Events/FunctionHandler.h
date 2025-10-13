#ifndef Events_StaticMethodHandler_H
#define Events_StaticMethodHandler_H

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
        explicit FunctionHandler(void(*function)(TParams...)) noexcept
        {
            _function = function;
        }

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