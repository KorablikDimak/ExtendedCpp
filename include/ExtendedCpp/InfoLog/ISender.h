#ifndef INFO_LOG_I_SENDER_H
#define INFO_LOG_I_SENDER_H

#include <map>
#include <memory>

#include <ExtendedCpp/InfoLog/LogLevel.h>
#include <ExtendedCpp/InfoLog/Parser.h>

/// @brief
namespace ExtendedCpp::InfoLog
{
    /// @brief
    class ISender
    {
    public:
        /// @brief
        using Ptr = std::shared_ptr<ISender>;

        /// @brief
        virtual ~ISender() = default;

        /// @brief 
        /// @param message 
        /// @param logLevel 
        /// @param tag
        virtual void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept = 0;

    protected:
        /// @brief
        std::map<std::string, std::string> Config;

        ///
        /// @param logLevel
        /// @return
        [[nodiscard]]
        bool ValidateLogLevel(LogLevel logLevel) const noexcept;

        ///
        /// @param tag
        /// @return
        [[nodiscard]]
        bool ValidateTag(const std::string& tag) const noexcept;
    };
}

#endif