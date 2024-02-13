#ifndef ValuesObject_H
#define ValuesObject_H

#include <Json/Json.h>

class ValuesObject
{
public:
    std::vector<int> intsField;
    std::vector<bool> boolsField;
    std::vector<std::string> stringsField;
};

inline void from_json(const Json::Json& json, ValuesObject& object)
{
    VALUES_FROM_JSON(intsField)
    VALUES_FROM_JSON(boolsField)
    VALUES_FROM_JSON(stringsField)
}

inline void to_json(Json::Json& json, const std::shared_ptr<ValuesObject>& object)
{
    VALUES_TO_JSON(intsField)
    VALUES_TO_JSON(boolsField)
    VALUES_TO_JSON(stringsField)
}

#endif