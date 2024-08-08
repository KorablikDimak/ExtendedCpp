#ifndef InfoLog_ConsoleSender_H
#define InfoLog_ConsoleSender_H

#include <InfoLog/ISender.h>

namespace InfoLog
{
    class ConsoleSender final : public ISender
    {
    public:
        explicit ConsoleSender(const std::map<std::string, std::string>& config) noexcept;
        ~ConsoleSender() override = default;

    protected:
        void Send(const std::string& message, LogLevel logLevel, const std::string& tag) noexcept override;
    };
}

#endif