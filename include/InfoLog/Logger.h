#ifndef InfoLog_Logger_H
#define InfoLog_Logger_H

#include <vector>
#include <map>
#include <memory>
#include <mutex>

#include <InfoLog/ConsoleSender.h>
#include <InfoLog/LogLevel.h>
#include <InfoLog/Converter.h>
#include <InfoLog/Configuration.h>

typedef std::vector<InfoLog::ISender::Ptr> Senders;

namespace InfoLog
{
    class Logger final
    {
    private:
        std::unique_ptr<Senders> _senders;
        std::mutex _loggerMutex;

    public:
        typedef std::shared_ptr<Logger> Ptr;

        Logger();

        template<typename T>
        explicit Logger(const T& fileName)
        {
            auto configuration = std::make_unique<Configuration>(fileName);
            _senders = std::make_unique<Senders>();

            for (const std::map<std::string, std::string>& config : configuration->GetConfigs())
                AddSender(config);
        }

        explicit Logger(const Configuration::Ptr& configuration);

        ~Logger() = default;

        void AddSender(const std::map<std::string, std::string>& config) const;
        void AddSender(const ISender::Ptr& sender) const;
        void RemoveSender(const ISender::Ptr& sender) const;

        template<typename T>
        void Trace(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Trace, tag);
            }
        }

        template<typename T>
        void Debug(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Debug, tag);
            }
        }

        template<typename T>
        void Info(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Info, tag);
            }
        }

        template<typename T>
        void Warning(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Warning, tag);
            }
        }

        template<typename T>
        void Error(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Error, tag);
            }
        }

        template<typename T>
        void Critical(const T& message, const std::string& tag = "")
        {
            for (const ISender::Ptr& sender : *_senders)
            {
                std::lock_guard<std::mutex> lock(_loggerMutex);
                sender->Send(InfoLog::ToString(message), LogLevel::Critical, tag);
            }
        }
    };
}

#endif