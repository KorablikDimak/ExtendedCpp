#ifndef INFO_LOG_CONFIGURATION_H
#define INFO_LOG_CONFIGURATION_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <ExtendedCpp/InfoLog/Converter.h>
#include <ExtendedCpp/Xml.h>

/// @brief
namespace ExtendedCpp::InfoLog
{
    /// @brief
    class Configuration final
    {
    private:
        std::vector<std::map<std::string, std::string>> _configs;

    public:
        /// @brief
        using Ptr = std::shared_ptr<Configuration>;

        /// @brief 
        /// @param configuration
        Configuration(const Configuration& configuration) noexcept = default;

        /// @brief 
        /// @param configuration
        Configuration(Configuration&& configuration) noexcept = default;

        /// @brief 
        /// @tparam T 
        /// @param fileName
        template<Concepts::ConvertableToString T>
        explicit Configuration(const T& fileName)
        {
            const std::string fileNameString = ToString(fileName);

            Xml::xml_document document;

            if (const Xml::xml_parse_result result = document.load_file(fileNameString.c_str()))
            {
                for (const pugi::xml_node target : document.child("logger").child("targets").children("target"))
                {
                    std::map<std::string, std::string> config;

                    for (const pugi::xml_attribute attribute : target.attributes())
                    {
                        std::string attributeName = attribute.name();
                        ToLowerCase(attributeName);
                        config.insert(std::make_pair(std::move(attributeName), attribute.value()));
                    }

                    _configs.push_back(config);
                }
            }
            else
            {
                std::cout << fileNameString.c_str() <<  " parse FAILED!\n";
                std::cout << "Error description: " << result.description() << '\n';
            }
        }

        /// @brief 
        /// @param configuration 
        /// @return
        Configuration& operator=(const Configuration& configuration) noexcept = default;

        /// @brief 
        /// @param configuration 
        /// @return
        Configuration& operator=(Configuration&& configuration) noexcept = default;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::vector<std::map<std::string, std::string>> GetConfigs() const noexcept
        {
            return _configs;
        }
    };
}

#endif