#include <InfoLog/Logger.h>
#include <InfoLog/FileSender.h>

InfoLog::Logger::Logger()
{
    _senders = std::make_unique<Senders>();
}

InfoLog::Logger::Logger(const Configuration::Ptr& configuration) noexcept
{
    _senders = std::make_unique<Senders>();

    const auto configs = configuration->GetConfigs();

    for (const std::map<std::string, std::string>& config : configs)
        AddSender(config);
}

void InfoLog::Logger::AddSender(const std::map<std::string, std::string>& config) noexcept
{
    std::string logSenderName;

    if (config.contains("sender")) logSenderName = ToLowerCase(config.at("sender"));
    else if (config.contains("logsender")) logSenderName = ToLowerCase(config.at("logsender"));
    else return;

    if (logSenderName == "file") _senders->push_back(std::make_shared<FileSender>(config));
    else if (logSenderName == "filesender") _senders->push_back(std::make_shared<FileSender>(config));
    else if (logSenderName == "console") _senders->push_back(std::make_shared<ConsoleSender>(config));
    else if (logSenderName == "consolesender") _senders->push_back(std::make_shared<ConsoleSender>(config));
}

void InfoLog::Logger::AddSender(const ISender::Ptr& sender) noexcept
{
    _senders->push_back(sender);
}

void InfoLog::Logger::RemoveSender(const ISender::Ptr& sender) noexcept
{
    for (int i = 0; i < _senders->size(); ++i)
        if (sender.get() == (*_senders)[i].get()) _senders->erase(_senders->begin() + i);
}
