#include "TestSender.h"

#include <iostream>

TestSender::TestSender(const std::map<std::string, std::string> &) {}

void TestSender::Send(const std::string &, ExtendedCpp::InfoLog::LogLevel, const std::string &) const noexcept
{
    std::cout << "Test sender ok" << std::endl;
}