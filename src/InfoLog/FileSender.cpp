#include <fstream>
#include <filesystem>
#include <iostream>

#include <ExtendedCpp/InfoLog/FileSender.h>
#include <ExtendedCpp/InfoLog/Parser.h>

ExtendedCpp::InfoLog::FileSender::FileSender(const std::map<std::string, std::string>& config)
{
    Config = config;
    if (!Config.contains("filepath"))
        throw std::invalid_argument("Config does not have attribute \"filepath\"");
    _filePath = Config.at("filepath");
}

void ExtendedCpp::InfoLog::FileSender::Send(const std::string& message, const LogLevel logLevel, const std::string& tag) const noexcept
{
    if (!ValidateLogLevel(logLevel)) return;
    if (!ValidateTag(tag)) return;
    if (!Config.contains("layout")) return;

    std::string filePath = _filePath;
    Parser::ParseLayout(filePath, message, logLevel, tag);

    try
    {
        std::filesystem::path layoutPath(filePath);
        std::lock_guard lock(_mutex);
        if (!exists(layoutPath.parent_path()))
            create_directories(layoutPath.parent_path());
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return;
    }

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    std::lock_guard lock(_mutex);
    std::ofstream fileStream(filePath, std::ios::app);
    if (fileStream.is_open())
    {
        fileStream << layout << std::endl;
        fileStream.close();
    }
}