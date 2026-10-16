#ifndef INFO_LOG_CONSOLE_SENDER_H
#define INFO_LOG_CONSOLE_SENDER_H

#include <mutex>

#include <ExtendedCpp/InfoLog/ISender.h>

/// @brief
namespace ExtendedCpp::InfoLog
{
    /// @brief
    class ConsoleSender final : public ISender
    {
    private:
        mutable std::mutex _mutex;

    public:
        /// @brief 
        /// @param config
        explicit ConsoleSender(const std::map<std::string, std::string>& config) noexcept;

        /// @brief
        ~ConsoleSender() override = default;

        /// @brief 
        /// @param message 
        /// @param logLevel 
        /// @param tag
        void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept override;
    };
}

#endif