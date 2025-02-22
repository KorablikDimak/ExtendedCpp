#ifndef InfoLog_ISender_H
#define InfoLog_ISender_H

#include <map>
#include <memory>

#include <ExtendedCpp/InfoLog/Parser.h>
#include <ExtendedCpp/InfoLog/LogLevel.h>

namespace ExtendedCpp::InfoLog
{
    /// @brief 
    class ISender
    {
    public:
        /// @brief 
        typedef std::shared_ptr<ISender> Ptr;

        /// @brief 
        ISender() noexcept = default;

        /// @brief 
        virtual ~ISender() = default;

        /// @brief 
        /// @param message 
        /// @param logLevel 
        /// @param tag 
        virtual void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept = 0;

    protected:
        std::map<std::string, std::string> Config;

        [[nodiscard]]
        bool ValidateLogLevel(LogLevel logLevel) const noexcept;
        [[nodiscard]]
        bool ValidateTag(const std::string& tag) const noexcept;
    };
}

#endif