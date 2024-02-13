#include <InfoLog/LoggerFactory.h>

InfoLog::LoggerFactory::LoggerFactory(std::unique_ptr<Configuration>& configuration)
{
    _configuration.swap(configuration);
}

void InfoLog::LoggerFactory::SetConfiguration(std::unique_ptr<Configuration>& configuration)
{
    _configuration.swap(configuration);
}

InfoLog::Logger::Ptr InfoLog::LoggerFactory::CreateLogger() const
{
    auto logger = std::make_shared<Logger>();

    for (const std::map<std::string, std::string>& config : _configuration->GetConfigs())
        logger->AddSender(config);

    return logger;
}
