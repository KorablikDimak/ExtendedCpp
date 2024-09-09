#ifndef Reflection_TypeTraits_H
#define Reflection_TypeTraits_H

#include <utility>
#include <tuple>
#include <typeindex>
#include <vector>
#include <memory>

namespace Reflection
{
    template<typename... TParams>
    std::vector<std::type_index> ToTypeIndexes() noexcept
    {
        std::vector<std::type_index> typeIndexes;
        typeIndexes.reserve(sizeof...(TParams));
        (typeIndexes.emplace_back(typeid(TParams)), ...);
        return typeIndexes;
    }

    template<typename T, typename Enable = void>
    struct IsSharedPtr
    {
        enum { value = false };
    };

    template<typename T>
    struct IsSharedPtr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::shared_ptr<typename T::element_type>>::value>::type>
    {
        enum { value = true };
    };
}

#endif