#include <fstream>
#include <filesystem>

#include <InfoLog/FileSender.h>

InfoLog::FileSender::FileSender(const std::map<std::string, std::string>& config)
{
    Config = config;
    _filePath = Config.at("filepath");
}

void InfoLog::FileSender::Send(const std::string& message, const LogLevel logLevel, const std::string& tag)
{
    if (!ValidateLogLevel(logLevel)) return;
    if (!ValidateTag(tag)) return;

    Parser::ParseLayout(_filePath, message, logLevel, tag);
    std::filesystem::path layoutPath(_filePath);
    if (!exists(layoutPath.parent_path()))
        create_directories(layoutPath.parent_path());

    std::string layout = Config.at("layout");
    Parser::ParseLayout(layout, message, logLevel, tag);

    std::ofstream fileStream(_filePath, std::ios::app);
	if (fileStream.is_open()) fileStream << layout << std::endl;
	fileStream.close();
}