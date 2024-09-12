#ifndef InfoLog_LoggerFactory_H
#define InfoLog_LoggerFactory_H

#include <ExtendedCpp/InfoLog/Logger.h>

namespace ExtendedCpp::InfoLog
{
    class LoggerFactory final
    {
    private:
        Configuration _configuration;

    public:
        template<Concepts::ConvertableToString T>
        explicit LoggerFactory(const T& fileName) : _configuration(fileName) {}

        LoggerFactory(const LoggerFactory& factory) noexcept = default;
        LoggerFactory(LoggerFactory&& factory) noexcept : _configuration(std::move(factory._configuration)) {}

        explicit LoggerFactory(const Configuration& configuration) noexcept;
        explicit LoggerFactory(Configuration&& configuration) noexcept;

        ~LoggerFactory() = default;

        template<Concepts::ConvertableToString T>
        void SetConfiguration(const T& fileName)
        {
            _configuration = Configuration(fileName);
        }

        template<Concepts::ConvertableToString T>
        void SetConfiguration(T&& fileName)
        {
            _configuration = Configuration(std::forward<T>(fileName));
        }

        void SetConfiguration(const Configuration& configuration) noexcept;
        void SetConfiguration(Configuration&& configuration) noexcept;

        [[nodiscard]]
        Logger::Ptr CreateLogger() const noexcept;
    };
}

#endif