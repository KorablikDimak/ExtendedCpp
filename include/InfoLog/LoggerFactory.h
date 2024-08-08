#ifndef InfoLog_LoggerFactory_H
#define InfoLog_LoggerFactory_H

#include <InfoLog/Logger.h>

namespace InfoLog
{
    class LoggerFactory final
    {
    private:
        std::unique_ptr<Configuration> _configuration;

    public:
        template<typename T>
        explicit LoggerFactory(const T& fileName) noexcept
        {
            _configuration = std::make_unique<Configuration>(fileName);
        }

        explicit LoggerFactory(std::unique_ptr<Configuration>& configuration) noexcept;

        ~LoggerFactory() = default;

        template<typename T>
        void SetConfiguration(const T& fileName) noexcept
        {
            _configuration = std::make_unique<Configuration>(fileName);
        }

        void SetConfiguration(std::unique_ptr<Configuration>& configuration) noexcept;

        [[nodiscard]]
        Logger::Ptr CreateLogger() const noexcept;
    };
}

#endif