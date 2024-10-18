#ifndef Json_JsonMacros_H
#define Json_JsonMacros_H

#include <ExtendedCpp/Json/json.hpp>
#include <ExtendedCpp/Json/Concepts.h>

#define VALUE_FROM_JSON(field) \
[]<typename TObject>(const ExtendedCpp::Json& json, TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<TObject>) \
        object->field = json.value<decltype(object->field)>(#field, decltype(object->field)()); \
    else \
        object.field = json.value<decltype(object.field)>(#field, decltype(object.field)()); \
}(json, object);

#define VALUES_FROM_JSON(field) \
[]<typename TObject>(const ExtendedCpp::Json& json, TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<TObject>) \
        object->field = json.value<decltype(object->field)>(#field, decltype(object->field)()); \
    else \
        object.field = json.value<decltype(object.field)>(#field, decltype(object.field)()); \
}(json, object);

#define OBJECT_FROM_JSON(field) \
[]<typename TObject>(const ExtendedCpp::Json& json, TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<TObject>) \
    { \
        if (json.contains(#field)) \
        { \
            if (object->field.get() == nullptr) \
                object->field = std::make_shared<std::remove_reference_t<decltype(*object->field)>>(); \
            *object->field = json.at(#field).get<std::remove_reference_t<decltype(*object->field)>>(); \
        } \
    } \
    else \
    { \
        if (json.contains(#field)) \
        { \
            if (object.field.get() == nullptr) \
                object.field = std::make_shared<std::remove_reference_t<decltype(*object.field)>>(); \
            *object.field = json.at(#field).get<std::remove_reference_t<decltype(*object.field)>>(); \
        } \
    } \
}(json, object);

#define OBJECTS_FROM_JSON(field) \
[]<typename TObject>(const ExtendedCpp::Json& json, TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<decltype(object)>) \
    { \
        if (json.contains(#field)) \
            for (const nlohmann::json& element : json.at(#field)) \
            { \
                typename decltype(object->field)::value_type elementPtr; \
                elementPtr = std::make_shared<std::remove_reference_t<decltype(*elementPtr)>>(); \
                *elementPtr = element.get<std::remove_reference_t<decltype(*elementPtr)>>(); \
                object->field.push_back(elementPtr); \
            } \
    } \
    else \
    { \
        if (json.contains(#field)) \
            for (const nlohmann::json& element : json.at(#field)) \
            { \
                typename decltype(object.field)::value_type elementPtr; \
                elementPtr = std::make_shared<std::remove_reference_t<decltype(*elementPtr)>>(); \
                *elementPtr = element.get<std::remove_reference_t<decltype(*elementPtr)>>(); \
                object.field.push_back(elementPtr); \
            } \
    } \
}(json, object);

#define VALUE_TO_JSON(field) \
[]<typename TObject>(ExtendedCpp::Json& json, const TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<decltype(object)>) \
        json[#field] = object->field; \
    else \
        json[#field] = object.field; \
}(json, object);

#define VALUES_TO_JSON(field) \
[]<typename TObject>(ExtendedCpp::Json& json, const TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<decltype(object)>) \
        json[#field] = object->field; \
    else \
        json[#field] = object.field; \
}(json, object);

#define OBJECT_TO_JSON(field) \
[]<typename TObject>(ExtendedCpp::Json& json, const TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<decltype(object)>) \
    { \
        if (object->field.get() != nullptr) \
            json[#field] = object->field; \
    } \
    else \
    { \
        if (object.field.get() != nullptr) \
            json[#field] = object.field; \
    } \
}(json, object);

#define OBJECTS_TO_JSON(field) \
[]<typename TObject>(ExtendedCpp::Json& json, const TObject& object) \
{ \
    if constexpr (ExtendedCpp::Concepts::IsSharedPtr<decltype(object)>) \
        for (const auto& element : object->field) \
            json[#field].push_back(element); \
    else \
        for (const auto& element : object.field) \
            json[#field].push_back(element); \
}(json, object);

#endif