#ifndef INFO_LOG_LOG_LEVEL_H
#define INFO_LOG_LOG_LEVEL_H

#include <cstdint>

/// @brief
namespace ExtendedCpp::InfoLog
{
    /// @brief
    enum class LogLevel : std::uint8_t
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
}

#endif