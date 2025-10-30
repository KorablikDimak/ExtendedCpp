#ifndef Events_FunctionTraits_H
#define Events_FunctionTraits_H

#include <ExtendedCpp/Events/FunctorHandler.h>

namespace ExtendedCpp::Events
{
	///
	/// @tparam TFunctor
	template<typename TFunctor>
	struct FunctorTraits : FunctorTraits<decltype(&TFunctor::operator())> {};

	///
	/// @tparam TFunctor
	/// @tparam TParams
	template<typename TFunctor, typename... TParams>
	struct FunctorTraits<void(TFunctor::*)(TParams...) const>
	{
		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(const TFunctor& functor) noexcept
		{
			return std::make_shared<FunctorHandler<TFunctor, TParams...>>(functor);
		}

		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(TFunctor&& functor) noexcept
		{
			return std::make_shared<FunctorHandler<TFunctor, TParams...>>(std::forward<TFunctor>(functor));
		}
	};

	///
	/// @tparam TFunctor
	/// @tparam TParams
	template<typename TFunctor, typename... TParams>
	struct FunctorTraits<void(TFunctor::*)(TParams...)>
	{
		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(TFunctor&& functor) noexcept
		{
			return std::make_shared<FunctorHandler<TFunctor, TParams...>>(std::forward<TFunctor>(functor));
		}
	};

	///
	/// @tparam TFunctor
	/// @tparam TParams
	template<typename TFunctor, typename... TParams>
	struct FunctorTraits<std::function<TFunctor(TParams...)>>
	{
		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(const std::function<TFunctor(TParams...)>& functor) noexcept
		{
			return std::make_shared<FunctorHandler<std::function<TFunctor(TParams...)>, TParams...>>(functor);
		}

		static std::shared_ptr<IEventHandler<TParams...>> CreateFunctorHandler(std::function<TFunctor(TParams...)>&& functor) noexcept
		{
			return std::make_shared<FunctorHandler<std::function<TFunctor(TParams...)>, TParams...>>(std::move(functor()));
		}
	};
}

#endif