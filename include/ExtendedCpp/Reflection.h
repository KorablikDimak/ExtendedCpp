#ifndef Reflection_Reflection_H
#define Reflection_Reflection_H

#include <optional>

#include <ExtendedCpp/Reflection/Assembly.h>
#include <ExtendedCpp/Reflection/Concepts.h>

namespace ExtendedCpp::Reflection
{
    /// @brief 
    /// @param typeName 
    /// @return 
    std::vector<TypeInfo> GetType(const std::string& typeName) noexcept;

    /// @brief 
    /// @param typeIndex 
    /// @return 
    std::optional<TypeInfo> GetType(std::type_index typeIndex) noexcept;

    /// @brief 
    /// @tparam T 
    /// @return 
    template<Concepts::HasMetaInfo T>
    TypeInfo GetType() noexcept
    {
        return T::MetaInfo;
    }
}

#endif