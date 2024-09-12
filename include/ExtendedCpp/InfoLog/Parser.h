#ifndef InfoLog_Parser_H
#define InfoLog_Parser_H

#include <chrono>
#include <string>

#include <ExtendedCpp/InfoLog/LogLevel.h>

namespace ExtendedCpp::InfoLog
{
    class Parser final
    {
    public:
        Parser() noexcept = default;
        ~Parser() = default;

        static void ParseLayout(std::string& layout, const std::string& message, LogLevel logLevel, const std::string& tag) noexcept;
    };
}

#endif