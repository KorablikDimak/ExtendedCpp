#ifndef InfoLog_Configuration_H
#define InfoLog_Configuration_H

#include <algorithm>
#include <map>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>

#include <Xml/Xml.h>
#include <InfoLog/Converter.h>

namespace InfoLog
{
    class Configuration final
    {
    private:
        std::vector<std::map<std::string, std::string>> _configs;

    public:
        typedef std::shared_ptr<Configuration> Ptr;

        template<typename T>
        explicit Configuration(const T& fileName)
        {
            std::stringstream stream;
            stream << fileName;
            const std::string fileNameString = stream.str();

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

        ~Configuration() = default;

        std::vector<std::map<std::string, std::string>> GetConfigs() noexcept;
    };
}

#endif