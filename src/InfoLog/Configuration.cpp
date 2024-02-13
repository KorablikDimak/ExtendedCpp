#include <InfoLog/Configuration.h>

std::vector<std::map<std::string, std::string>> InfoLog::Configuration::GetConfigs()
{
    return _configs;
}