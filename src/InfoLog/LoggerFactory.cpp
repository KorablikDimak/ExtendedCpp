#include <ExtendedCpp/InfoLog/LoggerFactory.h>

ExtendedCpp::InfoLog::LoggerFactory::LoggerFactory(const Configuration& configuration) noexcept :
    _configuration(configuration) {}

ExtendedCpp::InfoLog::LoggerFactory::LoggerFactory(Configuration&& configuration) noexcept :
    _configuration(std::move(configuration)) {}

void ExtendedCpp::InfoLog::LoggerFactory::SetConfiguration(const Configuration& configuration) noexcept
{
    _configuration = configuration;
}

void ExtendedCpp::InfoLog::LoggerFactory::SetConfiguration(Configuration&& configuration) noexcept
{
    _configuration = std::move(configuration);
}

ExtendedCpp::InfoLog::Logger::Ptr ExtendedCpp::InfoLog::LoggerFactory::CreateLogger() const noexcept
{
    auto logger = std::make_shared<Logger>();

    for (const std::map<std::string, std::string>& config : _configuration.GetConfigs())
        logger->AddSender(config);

    return logger;
}