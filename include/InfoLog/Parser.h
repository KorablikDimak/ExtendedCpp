#ifndef InfoLog_Parser_H
#define InfoLog_Parser_H

#include <chrono>
#include <string>

#include <InfoLog/LogLevel.h>

namespace InfoLog
{
    class Parser
    {
    public:
        Parser() = default;
        ~Parser() = default;

        static void ParseLayout(std::string& layout, const std::string& message, LogLevel logLevel, const std::string& tag);
    };
}

#endif