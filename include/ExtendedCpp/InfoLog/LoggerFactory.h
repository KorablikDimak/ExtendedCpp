#ifndef InfoLog_LoggerFactory_H
#define InfoLog_LoggerFactory_H

#include <ExtendedCpp/InfoLog/Logger.h>

/// @brief 
namespace ExtendedCpp::InfoLog
{
    /// @brief 
    class LoggerFactory final
    {
    private:
        Configuration _configuration;

    public:
        /// @brief 
        /// @tparam T 
        /// @param fileName 
        template<Concepts::ConvertableToString T>
        explicit LoggerFactory(const T& fileName) : _configuration(fileName) {}

        /// @brief 
        /// @param factory 
        LoggerFactory(const LoggerFactory& factory) noexcept = default;

        /// @brief 
        /// @param factory 
        LoggerFactory(LoggerFactory&& factory) noexcept : _configuration(std::move(factory._configuration)) {}

        /// @brief 
        /// @param configuration 
        explicit LoggerFactory(const Configuration& configuration) noexcept;

        /// @brief 
        /// @param configuration 
        explicit LoggerFactory(Configuration&& configuration) noexcept;

        /// @brief 
        ~LoggerFactory() = default;

        /// @brief 
        /// @tparam T 
        /// @param fileName 
        template<Concepts::ConvertableToString T>
        void SetConfiguration(const T& fileName)
        {
            _configuration = Configuration(fileName);
        }

        /// @brief 
        /// @tparam T 
        /// @param fileName 
        template<Concepts::ConvertableToString T>
        void SetConfiguration(T&& fileName)
        {
            _configuration = Configuration(std::forward<T>(fileName));
        }

        /// @brief 
        /// @param configuration 
        void SetConfiguration(const Configuration& configuration) noexcept;

        /// @brief 
        /// @param configuration 
        void SetConfiguration(Configuration&& configuration) noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        Logger::Ptr CreateLogger() const noexcept;
    };
}

#endif