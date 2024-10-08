#include <gtest/gtest.h>

#include <filesystem>

#include <ExtendedCpp/InfoLog/Parser.h>

TEST(ParserTests, ParseLayoutTest)
{
    // Average
    std::string layout = "{basedir}|{basedirectory}|{level}|{loglevel}|{message}";
    const std::string message = "testmessage";
    constexpr auto logLevel = ExtendedCpp::InfoLog::LogLevel::Info;

    const std::filesystem::path baseDirectory(std::filesystem::current_path());
    const std::string result = baseDirectory.string() + "|" + baseDirectory.string() + "|INFO|INFO|testmessage";

    // Act
    ExtendedCpp::InfoLog::Parser::ParseLayout(layout, message, logLevel, "");

    // Assert
    ASSERT_EQ(layout, result);
}