#ifndef Reflection_TypeTraits_H
#define Reflection_TypeTraits_H

#include <utility>
#include <tuple>
#include <typeindex>
#include <vector>
#include <memory>

namespace ExtendedCpp::Reflection
{
    /// @brief 
    /// @tparam ...TParams 
    /// @return 
    template<typename... TParams>
    std::vector<std::type_index> ToTypeIndexes() noexcept
    {
        std::vector<std::type_index> typeIndexes;
        typeIndexes.reserve(sizeof...(TParams));
        (typeIndexes.emplace_back(typeid(TParams)), ...);
        return typeIndexes;
    }
}

#endif