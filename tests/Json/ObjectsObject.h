#ifndef ObjectsObject_H
#define ObjectsObject_H

#include "ValueObject.h"

class ObjectsObject
{
public:
    std::vector<std::shared_ptr<ValueObject>> valueObjects;
};

inline void from_json(const Json::Json& json, ObjectsObject& object)
{
    OBJECTS_FROM_JSON(valueObjects)
}

inline void to_json(Json::Json& json, const std::shared_ptr<ObjectsObject>& object)
{
    OBJECTS_TO_JSON(valueObjects)
}

#endif