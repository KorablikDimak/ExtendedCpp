#include <InfoLog/LoggerFactory.h>

InfoLog::LoggerFactory::LoggerFactory(const Configuration& configuration) noexcept : _configuration(configuration) {}

InfoLog::LoggerFactory::LoggerFactory(Configuration&& configuration) noexcept : _configuration(std::move(configuration)) {}

void InfoLog::LoggerFactory::SetConfiguration(const Configuration& configuration) noexcept
{
    _configuration = configuration;
}

void InfoLog::LoggerFactory::SetConfiguration(Configuration&& configuration) noexcept
{
    _configuration = std::move(configuration);
}

InfoLog::Logger::Ptr InfoLog::LoggerFactory::CreateLogger() const noexcept
{
    auto logger = std::make_shared<Logger>();

    for (const std::map<std::string, std::string>& config : _configuration.GetConfigs())
        logger->AddSender(config);

    return logger;
}