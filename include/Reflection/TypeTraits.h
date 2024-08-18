#ifndef MethodTraits_MethodTraits_H
#define MethodTraits_MethodTraits_H

#include <utility>
#include <tuple>
#include <typeindex>

namespace Reflection
{
    template<typename TObject, typename TFunctor>
    struct MethodTraits;

    template<typename TObject, typename TFunctor, typename... Args>
    struct MethodTraits<TObject, TFunctor(Args...)> final
    {
        using ReturnType = decltype((std::declval<TObject>().*std::declval<TFunctor>())(std::declval<Args>()...));
    };

    template<typename... TParams>
    std::vector<std::type_index> ToTypeIndexes() noexcept
    {
        std::vector<std::type_index> typeIndexes;
        typeIndexes.reserve(sizeof...(TParams));
        (typeIndexes.push_back(typeid(TParams)), ...);
        return typeIndexes;
    }
}

#endif