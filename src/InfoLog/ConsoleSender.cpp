#include <ExtendedCpp/InfoLog/ConsoleSender.h>
#include <iostream>

ExtendedCpp::InfoLog::ConsoleSender::ConsoleSender(const std::map<std::string, std::string>& config) noexcept
{
    Config = config;
}

void ExtendedCpp::InfoLog::ConsoleSender::Send(const std::string& message, const LogLevel logLevel, const std::string& tag) const noexcept
{
    if (!ValidateLogLevel(logLevel)) return;
    if (!ValidateTag(tag)) return;
    if (!Config.contains("layout")) return;

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    std::scoped_lock lock(_mutex);
    std::cout << layout << '\n';
}