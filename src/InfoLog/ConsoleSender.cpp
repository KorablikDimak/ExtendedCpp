#include <iostream>
#include <InfoLog/ConsoleSender.h>

InfoLog::ConsoleSender::ConsoleSender(const std::map<std::string, std::string>& config)
{
    Config = config;
}

void InfoLog::ConsoleSender::Send(const std::string& message, const LogLevel logLevel, const std::string& tag)
{
    if (!ValidateLogLevel(logLevel)) return;
    if (!ValidateTag(tag)) return;

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    std::cout << layout << std::endl;
}