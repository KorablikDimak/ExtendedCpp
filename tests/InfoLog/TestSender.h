#ifndef TEST_SENDER_H
#define TEST_SENDER_H

#include <ExtendedCpp/InfoLog/ISender.h>
#include <ExtendedCpp/Reflection.h>

class TestSender final : public ExtendedCpp::InfoLog::ISender
{
public:
    explicit TestSender(const std::map<std::string, std::string>& config);
    ~TestSender() override = default;

    META(TestSender,
         CONSTRUCTOR(std::map<std::string, std::string>))

    void Send(const std::string& message, ExtendedCpp::InfoLog::LogLevel logLevel, const std::string& tag) const noexcept override;
};


#endif