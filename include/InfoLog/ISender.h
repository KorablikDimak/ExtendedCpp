#ifndef InfoLog_ISender_H
#define InfoLog_ISender_H

#include <map>
#include <memory>

#include <InfoLog/Parser.h>
#include <InfoLog/LogLevel.h>

namespace InfoLog
{
    class ISender
    {
    public:
        typedef std::shared_ptr<ISender> Ptr;

        ISender() = default;
        virtual ~ISender() = default;

        virtual void Send(const std::string& message, LogLevel logLevel, const std::string& tag) = 0;

    protected:
        std::map<std::string, std::string> Config;

        bool ValidateLogLevel(LogLevel logLevel) const;
        bool ValidateTag(const std::string& tag) const;
    };
}

#endif