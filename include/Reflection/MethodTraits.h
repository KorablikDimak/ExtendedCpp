#ifndef MethodTraits_MethodTraits_H
#define MethodTraits_MethodTraits_H

#include <utility>

template<typename TObject, typename TFunctor>
struct MethodTraits;

template<typename TObject, typename TFunctor, typename... Args>
struct MethodTraits<TObject, TFunctor(Args...)> final
{
    using ReturnType = decltype((std::declval<TObject>().*std::declval<TFunctor>())(std::declval<Args>()...));
};

#endif