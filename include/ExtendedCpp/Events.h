#ifndef Events_Events_H
#define Events_Events_H

#include <ExtendedCpp/Events/Event.h>
#include <ExtendedCpp/Events/FunctorHandler.h>
#include <ExtendedCpp/Events/MethodHandler.h>
#include <ExtendedCpp/Events/ConstMethodHandler.h>
#include <ExtendedCpp/Events/FunctionHandler.h>
#include <ExtendedCpp/Events/FunctorTraits.h>

/// @brief
namespace ExtendedCpp::Events
{
	template<typename TFunctor>
	auto CreateFunctorHandler(TFunctor&& functor) noexcept
	{
		return FunctorTraits<std::decay_t<TFunctor>>::CreateFunctorHandler(std::forward<TFunctor>(functor));
	}

	#define FUNCTOR_HANDLER(functor) \
	ExtendedCpp::Events::CreateFunctorHandler(functor)

	/// @brief
	/// @tparam TObject
	/// @tparam TParams
	/// @param object
	/// @param method
	/// @return
	template<typename TObject, typename ...TParams>
	std::shared_ptr<IEventHandler<TParams...>> CreateConstMethodHandler(TObject* object, void(TObject::*method)(TParams...) const) noexcept
	{
		return std::make_shared<ConstMethodHandler<TObject, TParams...>>(object, method);
	}

	#define CONST_METHOD_HANDLER(object, method) \
	ExtendedCpp::Events::CreateConstMethodHandler(object, method)

	#define THIS_CONST_METHOD_HANDLER(method) \
	CONST_METHOD_HANDLER(this, method)

	/// @brief
	/// @tparam TObject
	/// @tparam TParams
	/// @param object
	/// @param method
	/// @return
	template<typename TObject, typename ...TParams>
	std::shared_ptr<IEventHandler<TParams...>> CreateMethodHandler(TObject* object, void(TObject::*method)(TParams...)) noexcept
	{
		return std::make_shared<MethodHandler<TObject, TParams...>>(object, method);
	}

	#define METHOD_HANDLER(object, method) \
	ExtendedCpp::Events::CreateMethodHandler(object, method)

	#define THIS_METHOD_HANDLER(method) \
	METHOD_HANDLER(this, method)

	/// @brief
	/// @tparam TParams
	/// @param method
	/// @return
	template<typename ...TParams>
	std::shared_ptr<IEventHandler<TParams...>> CreateStaticMethodHandler(void(*method)(TParams...)) noexcept
	{
		return std::make_shared<FunctionHandler<TParams...>>(method);
	}

	#define STATIC_METHOD_HANDLER(method) \
	ExtendedCpp::Events::CreateStaticMethodHandler(method)

	/// @brief
	/// @tparam TParams
	/// @param function
	/// @return
	template<typename ...TParams>
	std::shared_ptr<IEventHandler<TParams...>> CreateFunctionHandler(void(*function)(TParams...)) noexcept
	{
		return std::make_shared<FunctionHandler<TParams...>>(function);
	}

	#define FUNCTION_HANDLER(function) \
	ExtendedCpp::Events::CreateFunctionHandler(function)
}

#endif