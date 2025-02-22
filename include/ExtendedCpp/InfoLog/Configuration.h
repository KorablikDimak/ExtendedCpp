#ifndef InfoLog_Configuration_H
#define InfoLog_Configuration_H

#include <algorithm>
#include <map>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>

#include <ExtendedCpp/Xml.h>
#include <ExtendedCpp/InfoLog/Converter.h>

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
        typedef std::shared_ptr<Configuration> Ptr;

        /// @brief 
        /// @param configuration 
        Configuration(const Configuration& configuration) noexcept;

        /// @brief 
        /// @param configuration 
        Configuration(Configuration&& configuration) noexcept;

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
                for (pugi::xml_node target : document.child("logger").child("targets").children("target"))
                {
                    std::map<std::string, std::string> config;

                    for (pugi::xml_attribute attribute : target.attributes())
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
                std::cout << "Error description: " << result.description() << std::endl;
            }
        }

        /// @brief 
        ~Configuration() = default;

        /// @brief 
        /// @param configuration 
        /// @return 
        Configuration& operator=(const Configuration& configuration) noexcept = default;

        /// @brief 
        /// @param configuration 
        /// @return 
        Configuration& operator=(Configuration&& configuration) noexcept
        {
            _configs = std::move(configuration._configs);
            return *this;
        }

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::vector<std::map<std::string, std::string>> GetConfigs() const noexcept;
    };
}

#endif