#include <iostream>
#include <ExtendedCpp/InfoLog/ConsoleSender.h>

ExtendedCpp::InfoLog::ConsoleSender::ConsoleSender(const std::map<std::string, std::string>& config) noexcept
{
    Config = config;
}

void ExtendedCpp::InfoLog::ConsoleSender::Send(const std::string& message, const ExtendedCpp::InfoLog::LogLevel logLevel, const std::string& tag) const noexcept
{
    if (!ValidateLogLevel(logLevel)) return;
    if (!ValidateTag(tag)) return;
    if (!Config.contains("layout")) return;

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    std::lock_guard lock(_mutex);
    std::cout << layout << std::endl;
}