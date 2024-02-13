#ifndef ValueObject_H
#define ValueObject_H

#include <Json/Json.h>

class ValueObject
{
public:
    int intField;
    bool boolField;
    std::string stringField;
};

inline void from_json(const Json::Json& json, ValueObject& object)
{
    VALUE_FROM_JSON(intField)
    VALUE_FROM_JSON(boolField)
    VALUE_FROM_JSON(stringField)
}

inline void to_json(Json::Json& json, const std::shared_ptr<ValueObject>& object)
{
    VALUES_TO_JSON(intField)
    VALUES_TO_JSON(boolField)
    VALUES_TO_JSON(stringField)
}

#endif