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

namespace InfoLog
{
    class Logger final
    {
    private:
        std::vector<InfoLog::ISender::Ptr> _senders;

    public:
        typedef std::shared_ptr<Logger> Ptr;

        Logger() noexcept = default;

        template<ConvertableToString T>
        explicit Logger(const T& fileName) noexcept
        {
            auto configuration = std::make_unique<Configuration>(fileName);
            for (const std::map<std::string, std::string>& config : configuration->GetConfigs())
                AddSender(config);
        }

        explicit Logger(const Configuration::Ptr& configuration) noexcept;

        ~Logger() = default;

        void AddSender(const std::map<std::string, std::string>& config) noexcept;
        void AddSender(const ISender::Ptr& sender) noexcept;
        void RemoveSender(const ISender::Ptr& sender) noexcept;

        template<Concepts::ConvertableToString T>
        void Trace(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Trace, tag);
        }

        template<Concepts::ConvertableToString T>
        void Debug(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Debug, tag);
        }

        template<Concepts::ConvertableToString T>
        void Info(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Info, tag);
        }

        template<Concepts::ConvertableToString T>
        void Warning(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Warning, tag);
        }

        template<Concepts::ConvertableToString T>
        void Error(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Error, tag);
        }

        template<Concepts::ConvertableToString T>
        void Critical(const T& message, const std::string& tag = "") const noexcept
        {
            for (const ISender::Ptr& sender : _senders)
                sender->Send(InfoLog::ToString(message), LogLevel::Critical, tag);
        }
    };
}

#endif