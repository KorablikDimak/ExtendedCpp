#include <fstream>
#include <filesystem>
#include <iostream>

#include <InfoLog/FileSender.h>
#include <InfoLog/Parser.h>

InfoLog::FileSender::FileSender(const std::map<std::string, std::string>& config)
{
    Config = config;
    if (!Config.contains("filepath")) throw std::invalid_argument("Config does not have attribute \"filepath\"");
    _filePath = Config.at("filepath");
}

void InfoLog::FileSender::Send(const std::string& message, const LogLevel logLevel, const std::string& tag) const noexcept
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
    catch (...)
    {
        std::cerr << "std::filesystem error" << std::endl;
        return;
    }

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    try
    {
        std::lock_guard lock(_mutex);
        std::ofstream fileStream(filePath, std::ios::app);
        if (fileStream.is_open()) fileStream << layout << std::endl;
        fileStream.close();
    }
    catch (...)
    {
        std::cerr << "std::ofstream error" << std::endl;
        return;
    }
}