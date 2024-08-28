#ifndef InfoLog_FileSender_H
#define InfoLog_FileSender_H

#include <mutex>

#include <InfoLog/ISender.h>

namespace InfoLog
{
    class FileSender final : public ISender
    {
    private:
        std::string _filePath;
        mutable std::mutex _mutex;

    public:
        explicit FileSender(const std::map<std::string, std::string>& config);
        ~FileSender() override = default;

    protected:
        void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept override;
    };
}

#endif