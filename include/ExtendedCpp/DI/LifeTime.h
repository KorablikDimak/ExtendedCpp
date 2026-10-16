#ifndef DI_LIFE_TIME_H
#define DI_LIFE_TIME_H

#include <cstdint>

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::DI
{
    /// @brief
    enum class LifeTime : std::uint8_t
    {
        Singleton,
        Transient,
        Scoped
    };
}

#endif