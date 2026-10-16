#ifndef INFO_LOG_PARSER_H
#define INFO_LOG_PARSER_H

#include <string>

#include <ExtendedCpp/InfoLog/LogLevel.h>

/// @brief
namespace ExtendedCpp::InfoLog
{
    /// @brief
    class Parser final
    {
    public:
        /// @brief
        Parser() noexcept = delete;

        /// @brief 
        /// @param layout 
        /// @param message 
        /// @param logLevel 
        /// @param tag
        static void ParseLayout(std::string& layout, const std::string& message,
                                LogLevel logLevel, const std::string& tag) noexcept;
    };
}

#endif