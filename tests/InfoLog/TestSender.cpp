#include "TestSender.h"

#include <iostream>

TestSender::TestSender(const std::map<std::string, std::string> &config) {}

void TestSender::Send(const std::string &message, ExtendedCpp::InfoLog::LogLevel logLevel, const std::string &tag) const noexcept
{
    std::cout << "Test sender ok" << std::endl;
}