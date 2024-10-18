#ifndef ValueObject_H
#define ValueObject_H

#include <ExtendedCpp/Json.h>

class ValueObject
{
public:
    int intField;
    bool boolField;
    std::string stringField;
};

inline void from_json(const ExtendedCpp::Json& json, ValueObject& object)
{
    VALUE_FROM_JSON(intField)
    VALUE_FROM_JSON(boolField)
    VALUE_FROM_JSON(stringField)
}

inline void to_json(ExtendedCpp::Json& json, const std::shared_ptr<ValueObject>& object)
{
    VALUE_TO_JSON(intField)
    VALUE_TO_JSON(boolField)
    VALUE_TO_JSON(stringField)
}

#endif