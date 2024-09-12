#include <ExtendedCpp/InfoLog/Configuration.h>

ExtendedCpp::InfoLog::Configuration::Configuration(const Configuration& configuration) noexcept
{
    _configs = configuration._configs;
}

ExtendedCpp::InfoLog::Configuration::Configuration(Configuration&& configuration) noexcept
{
    _configs = std::move(configuration._configs);
}

std::vector<std::map<std::string, std::string>> ExtendedCpp::InfoLog::Configuration::GetConfigs() const noexcept
{
    return _configs;
}