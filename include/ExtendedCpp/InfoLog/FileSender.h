#ifndef InfoLog_FileSender_H
#define InfoLog_FileSender_H

#include <mutex>

#include <ExtendedCpp/InfoLog/ISender.h>

namespace ExtendedCpp::InfoLog
{
    /// @brief 
    class FileSender final : public ISender
    {
    private:
        std::string _filePath;
        mutable std::mutex _mutex;

    public:
        /// @brief 
        /// @param config 
        explicit FileSender(const std::map<std::string, std::string>& config);

        /// @brief 
        ~FileSender() override = default;

    protected:
        /// @brief 
        /// @param message 
        /// @param logLevel 
        /// @param tag 
        void Send(const std::string& message, LogLevel logLevel, const std::string& tag) const noexcept override;
    };
}

#endif