#include "TestSender.h"

#include <iostream>

META_IMPL(TestSender,
          CONSTRUCTOR(std::map<std::string, std::string>))

TestSender::TestSender(const std::map<std::string, std::string> &config) {}

void TestSender::Send(const std::string &message, LogLevel logLevel, const std::string &tag) const noexcept
{
    std::cout << "Test sender ok" << std::endl;
}