#include <ExtendedCpp/InfoLog/ISender.h>

#include <ExtendedCpp/InfoLog/Converter.h>

bool ExtendedCpp::InfoLog::ISender::ValidateLogLevel(const LogLevel logLevel) const noexcept
{
    if (!Config.contains("minlevel")) return true;

    const std::string minLogLevelString = ToLowerCase(Config.at("minlevel"));
    auto minLogLevel = LogLevel::Trace;

    if (minLogLevelString == "debug") minLogLevel = LogLevel::Debug;
    else if (minLogLevelString == "info") minLogLevel = LogLevel::Info;
    else if (minLogLevelString == "warning") minLogLevel = LogLevel::Warning;
    else if (minLogLevelString == "error") minLogLevel = LogLevel::Error;
    else if (minLogLevelString == "critical") minLogLevel = LogLevel::Critical;

    return logLevel >= minLogLevel;
}

bool ExtendedCpp::InfoLog::ISender::ValidateTag(const std::string& tag) const noexcept
{
    if (!Config.contains("tag")) return true;
    return Config.at("tag") == tag;
}
