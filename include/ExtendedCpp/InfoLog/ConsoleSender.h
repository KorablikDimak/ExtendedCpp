#ifndef InfoLog_ConsoleSender_H
#define InfoLog_ConsoleSender_H

#include <mutex>

#include <ExtendedCpp/InfoLog/ISender.h>

namespace ExtendedCpp::InfoLog
{
    class ConsoleSender final : public ISender
    {
    private:
        mutable std::mutex _mutex;

    public:
        explicit ConsoleSender(const std::map<std::string, std::string>& config) noexcept;
        ~ConsoleSender() override = default;

    protected:
        void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept override;
    };
}

#endif