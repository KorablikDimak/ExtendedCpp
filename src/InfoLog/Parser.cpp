#include <filesystem>
#include <chrono>

#include <ExtendedCpp/InfoLog/Parser.h>
#include <ExtendedCpp/InfoLog/Converter.h>

void ExtendedCpp::InfoLog::Parser::ParseLayout(std::string& layout, const std::string& message, const LogLevel logLevel, const std::string& tag) noexcept
{
    const auto currentTime = std::chrono::system_clock::now();
    const time_t time = std::chrono::system_clock::to_time_t(currentTime);
    const std::unique_ptr<tm> timeStructure(new tm());

#if defined(_WIN32)
    localtime_s(timeStructure.get(), &time);
#elif defined(__linux__)
    localtime_r(&time, timeStructure.get());
#elif defined(__APPLE__)
    localtime_r(&time, timeStructure.get());
#endif

    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(currentTime);
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - seconds);

    ReplaceAll(layout, "{years}", std::to_string(timeStructure->tm_year + 1900));
    ReplaceAll(layout, "{months}", std::to_string(timeStructure->tm_mon + 1));
    ReplaceAll(layout, "{days}", std::to_string(timeStructure->tm_mday));

    ReplaceAll(layout, "{year}", std::to_string(timeStructure->tm_year + 1900));
    ReplaceAll(layout, "{month}", std::to_string(timeStructure->tm_mon + 1));
    ReplaceAll(layout, "{day}", std::to_string(timeStructure->tm_mday));

    ReplaceAll(layout, "{yyyy}", std::to_string(timeStructure->tm_year + 1900));
    ReplaceAll(layout, "{yy}", std::to_string(timeStructure->tm_year - 100));
    ReplaceAll(layout, "{mm}", std::to_string(timeStructure->tm_mon + 1));
    ReplaceAll(layout, "{dd}", std::to_string(timeStructure->tm_mday));

    ReplaceAll(layout, "{hours}", std::to_string(timeStructure->tm_hour));
    ReplaceAll(layout, "{minutes}", std::to_string(timeStructure->tm_min));
    ReplaceAll(layout, "{seconds}", std::to_string(timeStructure->tm_sec));
    ReplaceAll(layout, "{milliseconds}", std::to_string(milliseconds.count()));

    ReplaceAll(layout, "{hour}", std::to_string(timeStructure->tm_hour));
    ReplaceAll(layout, "{minute}", std::to_string(timeStructure->tm_min));
    ReplaceAll(layout, "{second}", std::to_string(timeStructure->tm_sec));
    ReplaceAll(layout, "{millisecond}", std::to_string(milliseconds.count()));

    std::string logLevelString = "TRACE";
    if (logLevel == LogLevel::Debug) logLevelString = "DEBUG";
    else if (logLevel == LogLevel::Info) logLevelString = "INFO";
    else if (logLevel == LogLevel::Warning) logLevelString = "WARNING";
    else if (logLevel == LogLevel::Error) logLevelString = "ERROR";
    else if (logLevel == LogLevel::Critical) logLevelString = "CRITICAL";

    ReplaceAll(layout, "{level}", logLevelString);
    ReplaceAll(layout, "{loglevel}", logLevelString);

    try
    {
        const std::filesystem::path baseDirectory = std::filesystem::current_path();
        ReplaceAll(layout, "{basedir}", baseDirectory.string());
        ReplaceAll(layout, "{basedirectory}", baseDirectory.string());
        ReplaceAll(layout, "{tag}", tag);
        ReplaceAll(layout, "{message}", message);
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return;
    }
}