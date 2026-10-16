#ifndef REFLECTION_TYPE_TRAITS_H
#define REFLECTION_TYPE_TRAITS_H

#include <typeindex>
#include <vector>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief 
    /// @tparam TParams
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