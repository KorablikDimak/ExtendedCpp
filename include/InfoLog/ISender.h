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

        ISender() noexcept = default;
        virtual ~ISender() = default;

        virtual void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept = 0;

    protected:
        std::map<std::string, std::string> Config;

        [[nodiscard]]
        bool ValidateLogLevel(LogLevel logLevel) const noexcept;
        [[nodiscard]]
        bool ValidateTag(const std::string& tag) const noexcept;
    };
}

#endif