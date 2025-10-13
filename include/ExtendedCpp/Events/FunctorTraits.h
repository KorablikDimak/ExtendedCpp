#ifndef Events_FunctionTraits_H
#define Events_FunctionTraits_H

#include <ExtendedCpp/Events/FunctorHandler.h>

namespace ExtendedCpp::Events
{
	template<typename TFunctor>
	struct FunctorTraits : FunctorTraits<decltype(&TFunctor::operator())> {};

	template<typename TFunctor, typename... TParams>
	struct FunctorTraits<void(TFunctor::*)(TParams...) const>
	{
		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(const TFunctor& functor) noexcept
		{
			return std::make_shared<FunctorHandler<TFunctor, TParams...>>(functor);
		}

		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(TFunctor&& functor) noexcept
		{
			return std::make_shared<FunctorHandler<TFunctor, TParams...>>(std::move(functor));
		}
	};
}

#endif