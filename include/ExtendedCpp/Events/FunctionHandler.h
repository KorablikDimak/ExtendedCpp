#ifndef Events_LambdaHandler_H
#define Events_LambdaHandler_H

#include <functional>
#include <memory>

#include <ExtendedCpp/Events/IEventHandler.h>

namespace ExtendedCpp::Events
{
    template<typename ...TParams>
    class FunctionHandler final : public IEventHandler<TParams...>
    {
    private:
        std::function<void(TParams...)> _function;

    public:
        explicit FunctionHandler(std::function<void(TParams...)> function) noexcept
        {
            _function = function;
        }

        ~FunctionHandler() override = default;

        void Call(TParams... params) const override
        {
            _function(std::forward<TParams>(params)...);
        }

        bool IsEquals(std::function<void(TParams...)> function) const noexcept
        {
            typedef void(Fn)(TParams...);
            return _function.template target<Fn*>() == function.template target<Fn*>();
        }

    protected:
        bool IsEquals(const IEventHandler<TParams...>& other) const noexcept override
        {
            const auto* functionHandler = static_cast<const FunctionHandler*>(&other);
            return functionHandler->IsEquals(_function);
        }
    };

    template<typename ...TParams>
    std::shared_ptr<IEventHandler<TParams...>> CreateFunctionHandler(std::function<void(TParams...)> function) noexcept
    {
        return std::make_shared<FunctionHandler<TParams...>>(function);
    }
}

#define FUNCTION_HANDLER(function) \
ExtendedCpp::Events::CreateFunctionHandler(function)

#endif