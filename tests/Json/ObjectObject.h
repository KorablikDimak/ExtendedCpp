#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include "ValueObject.h"
#include "ValuesObject.h"

// ReSharper disable CppInconsistentNaming

class ObjectObject
{
public:
    std::shared_ptr<ValueObject> valueObject;
    std::shared_ptr<ValuesObject> valuesObject;
};

inline void from_json(const ExtendedCpp::Json& json, ObjectObject& object)
{
    OBJECT_FROM_JSON(valueObject)
    OBJECT_FROM_JSON(valuesObject)
}

inline void to_json(ExtendedCpp::Json& json, const std::shared_ptr<ObjectObject>& object)
{
    OBJECT_TO_JSON(valueObject)
    OBJECT_TO_JSON(valuesObject)
}

#endif