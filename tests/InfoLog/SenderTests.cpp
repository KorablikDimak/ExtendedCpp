#include <gtest/gtest.h>

#include <ExtendedCpp/InfoLog/LoggerFactory.h>

TEST(FileSenderTests, CreateLoggerTest)
{
    // Average
    const std::string xmlFilePath = "LogConfig.xml";

    // Act
    const auto factory = ExtendedCpp::InfoLog::LoggerFactory(xmlFilePath);
    const auto logger = factory.CreateLogger();

    // Assert
    ASSERT_TRUE(logger.get() != nullptr);
}

TEST(FileSenderTests, FileLogSendTest)
{
    // Average
    std::string xmlFilePath = "LogConfig.xml";

    // Act
    auto factory = ExtendedCpp::InfoLog::LoggerFactory(xmlFilePath);
    auto logger = factory.CreateLogger();

    // Assert
    ASSERT_NO_THROW(logger->Trace("FileLogSendTest Failed", "your tag"));
    ASSERT_NO_THROW(logger->Warning("FileLogSendTest Ok", "your tag"));
    ASSERT_NO_THROW(logger->Critical("FileLogSendTest Ok", "your tag"));
    ASSERT_NO_THROW(logger->Critical("FileLogSendTest Failed", "incorrect tag"));
}