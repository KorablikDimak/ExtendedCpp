#include <InfoLog/Configuration.h>

InfoLog::Configuration::Configuration(const Configuration& configuration) noexcept
{
    _configs = configuration._configs;
}

InfoLog::Configuration::Configuration(Configuration&& configuration) noexcept
{
    _configs = std::move(configuration._configs);
}

std::vector<std::map<std::string, std::string>> InfoLog::Configuration::GetConfigs() const noexcept
{
    return _configs;
}