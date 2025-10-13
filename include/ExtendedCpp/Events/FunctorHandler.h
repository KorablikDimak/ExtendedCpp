#ifndef Events_LambdaHandler_H
#define Events_LambdaHandler_H

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TParams
    template<typename TFunctor, typename ...TParams>
    requires std::same_as<std::invoke_result_t<TFunctor, TParams...>, void>
    class FunctorHandler final : public IEventHandler<TParams...>
    {
    private:
        TFunctor _functor;

    public:
        /// @brief 
        /// @param functor
        explicit FunctorHandler(const TFunctor& functor) noexcept
        {
            _functor = functor;
        }

        /// @brief
        /// @param functor
        explicit FunctorHandler(TFunctor&& functor) noexcept
        {
            _functor = std::move(functor);
        }

        /// @brief 
        ~FunctorHandler() override = default;

        /// @brief 
        /// @param params
        void Call(TParams&&... params) const override
        {
            _functor(std::forward<TParams>(params)...);
        }
    };
}

#endif