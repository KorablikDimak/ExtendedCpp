#ifndef TestSender_H
#define TestSender_H

#include <InfoLog/ISender.h>
#include <Reflection/Reflection.h>

class TestSender final : public InfoLog::ISender
{
public:
    explicit TestSender(const std::map<std::string, std::string>& config);
    ~TestSender() override = default;

    META(TestSender,
         CONSTRUCTOR(std::map<std::string, std::string>));

protected:
    void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept override;
};


#endif