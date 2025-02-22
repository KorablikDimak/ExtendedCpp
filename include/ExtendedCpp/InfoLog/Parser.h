#ifndef InfoLog_Parser_H
#define InfoLog_Parser_H

#include <string>

#include <ExtendedCpp/InfoLog/LogLevel.h>

/// @brief 
namespace ExtendedCpp::InfoLog
{
    /// @brief 
    class Parser final
    {
    public:
        Parser() noexcept = default;
        ~Parser() = default;

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