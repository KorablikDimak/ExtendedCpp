#ifndef InfoLog_Logger_H
#define InfoLog_Logger_H

#include <vector>
#include <map>
#include <memory>
#include <future>

#include <ExtendedCpp/InfoLog/ConsoleSender.h>
#include <ExtendedCpp/InfoLog/LogLevel.h>
#include <ExtendedCpp/InfoLog/Converter.h>
#include <ExtendedCpp/InfoLog/Configuration.h>

/// @brief 
namespace ExtendedCpp::InfoLog
{
    /// @brief 
    class Logger final
    {
    private:
        std::vector<InfoLog::ISender::Ptr> _senders;

        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag>
        void Send(TMessage&& message, LogLevel logLevel, TTag&& tag) const noexcept
        {
            std::vector<std::future<void>> _tasks;
            _tasks.reserve(_senders.size());

            for (const ISender::Ptr& sender : _senders)
                _tasks.push_back(std::async(std::launch::async,
                    [&](){ sender->Send(ToString(std::forward<TMessage>(message)), logLevel, ToString(std::forward<TTag>(tag))); }));
        }

    public:
        /// @brief 
        typedef std::shared_ptr<Logger> Ptr;

        /// @brief 
        Logger() noexcept = default;

        /// @brief 
        /// @tparam T 
        /// @param fileName 
        template<Concepts::ConvertableToString T>
        explicit Logger(T&& fileName) noexcept
        {
            auto configuration = std::make_unique<Configuration>(std::forward<T>(fileName));
            for (std::map<std::string, std::string>& config : configuration->GetConfigs())
                AddSender(std::move(config));
        }

        /// @brief 
        /// @param configuration 
        explicit Logger(const Configuration::Ptr& configuration) noexcept;

        /// @brief 
        ~Logger() = default;

        /// @brief 
        /// @param config 
        void AddSender(const std::map<std::string, std::string>& config) noexcept;

        /// @brief 
        /// @param config 
        void AddSender(std::map<std::string, std::string>&& config) noexcept;

        /// @brief 
        /// @param sender 
        void AddSender(const ISender::Ptr& sender) noexcept;

        /// @brief 
        /// @param sender 
        void RemoveSender(const ISender::Ptr& sender) noexcept;

        /// @brief 
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Trace(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Trace, std::forward<TTag>(tag));
        }

        /// @brief 
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Debug(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Debug, std::forward<TTag>(tag));
        }

        /// @brief 
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Info(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Info, std::forward<TTag>(tag));
        }

        /// @brief 
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Warning(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Warning, std::forward<TTag>(tag));
        }

        /// @brief 
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Error(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Error, std::forward<TTag>(tag));
        }

        /// @brief
        /// @tparam TMessage 
        /// @tparam TTag 
        /// @param message 
        /// @param tag 
        template<Concepts::ConvertableToString TMessage, Concepts::ConvertableToString TTag = std::string>
        void Critical(TMessage&& message, TTag&& tag = TTag()) const noexcept
        {
            Send(std::forward<TMessage>(message), LogLevel::Critical, std::forward<TTag>(tag));
        }
    };
}

#endif