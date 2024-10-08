#include <ExtendedCpp/InfoLog/Logger.h>
#include <ExtendedCpp/InfoLog/FileSender.h>
#include <ExtendedCpp/Reflection.h>

ExtendedCpp::InfoLog::Logger::Logger(const Configuration::Ptr& configuration) noexcept
{
    const auto configs = configuration->GetConfigs();
    for (const std::map<std::string, std::string>& config : configs)
        AddSender(config);
}

void ExtendedCpp::InfoLog::Logger::AddSender(const std::map<std::string, std::string>& config) noexcept
{
    std::string logSenderName;

    if (config.contains("sender")) logSenderName = config.at("sender");
    else if (config.contains("logsender")) logSenderName = config.at("logsender");
    else return;

    if (ToLowerCase(logSenderName) == "file") _senders.push_back(std::make_shared<FileSender>(config));
    else if (ToLowerCase(logSenderName) == "filesender") _senders.push_back(std::make_shared<FileSender>(config));
    else if (ToLowerCase(logSenderName) == "console") _senders.push_back(std::make_shared<ConsoleSender>(config));
    else if (ToLowerCase(logSenderName) == "consolesender") _senders.push_back(std::make_shared<ConsoleSender>(config));
    else
    {
        const std::vector<Reflection::TypeInfo> sendersInfo = Reflection::GetType(logSenderName);
        if (sendersInfo.empty()) return;

        for (const auto& senderInfo : sendersInfo)
        {
            const auto constructors = senderInfo.GetConstructors();
            if (constructors.empty()) continue;

            for (const auto& constructor : constructors)
                if (constructor->Parameters().size() == 1 && constructor->Parameters()[0] == typeid(config))
                {
                    std::shared_ptr<void> newSender = constructor->New(config);
                    if (!newSender) continue;
                    _senders.push_back(std::static_pointer_cast<ISender>(newSender));
                    return;
                }
        }
    }
}

void ExtendedCpp::InfoLog::Logger::AddSender(std::map<std::string, std::string>&& config) noexcept
{
    std::string logSenderName;

    if (config.contains("sender")) logSenderName = config.at("sender");
    else if (config.contains("logsender")) logSenderName = config.at("logsender");
    else return;

    if (ToLowerCase(logSenderName) == "file") _senders.push_back(std::make_shared<FileSender>(std::move(config)));
    else if (ToLowerCase(logSenderName) == "filesender") _senders.push_back(std::make_shared<FileSender>(std::move(config)));
    else if (ToLowerCase(logSenderName) == "console") _senders.push_back(std::make_shared<ConsoleSender>(std::move(config)));
    else if (ToLowerCase(logSenderName) == "consolesender") _senders.push_back(std::make_shared<ConsoleSender>(std::move(config)));
    else
    {
        const std::vector<Reflection::TypeInfo> sendersInfo = Reflection::GetType(logSenderName);
        if (sendersInfo.empty()) return;

        for (const auto& senderInfo : sendersInfo)
        {
            const auto constructors = senderInfo.GetConstructors();
            if (constructors.empty()) continue;

            for (const auto& constructor : constructors)
                if (constructor->Parameters().size() == 1 && constructor->Parameters()[0] == typeid(config))
                {
                    std::shared_ptr<void> newSender = constructor->New(config);
                    if (!newSender) continue;
                    _senders.push_back(std::static_pointer_cast<ISender>(newSender));
                    return;
                }
        }
    }
}

void ExtendedCpp::InfoLog::Logger::AddSender(const ISender::Ptr& sender) noexcept
{
    _senders.push_back(sender);
}

void ExtendedCpp::InfoLog::Logger::RemoveSender(const ISender::Ptr& sender) noexcept
{
    for (std::size_t i = 0; i < _senders.size(); ++i)
        if (sender.get() == _senders[i].get()) _senders.erase(_senders.begin() + i);
}