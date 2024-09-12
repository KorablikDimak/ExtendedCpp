#include <gtest/gtest.h>

#include <ExtendedCpp/Json.h>

#include "ObjectObject.h"
#include "ObjectsObject.h"

TEST(JsonTests, VALUE_FROM_JSON_Test)
{
    // Average
    std::string jsonString =
        R"({"intField": 1, "boolField": true, "stringField": "stringValue"})";
    const ExtendedCpp::Json json = ExtendedCpp::Json::parse(jsonString);

    // Act
    const auto [intField, boolField, stringField] = json.get<ValueObject>();

    // Assert
    ASSERT_EQ(intField, 1);
    ASSERT_EQ(boolField, true);
    ASSERT_EQ(stringField, "stringValue");
}

TEST(JsonTests, VALUES_FROM_JSON_Test)
{
    // Average
    const std::string jsonString =
        R"({"intsField": [1, 2, 3], "boolsField": [true, false, true], "stringsField": ["stringValue1", "stringValue2", "stringValue3"]})";
    const ExtendedCpp::Json json = ExtendedCpp::Json::parse(jsonString);

    // Act
    const auto [intsField, boolsField, stringsField] = json.get<ValuesObject>();

    // Assert
    ASSERT_EQ(intsField[0], 1);
    ASSERT_EQ(boolsField[0], true);
    ASSERT_EQ(stringsField[0], "stringValue1");

    ASSERT_EQ(intsField[1], 2);
    ASSERT_EQ(boolsField[1], false);
    ASSERT_EQ(stringsField[1], "stringValue2");

    ASSERT_EQ(intsField[2], 3);
    ASSERT_EQ(boolsField[2], true);
    ASSERT_EQ(stringsField[2], "stringValue3");
}

TEST(JsonTests, OBJECT_FROM_JSON_Test)
{
    // Average
    const std::string jsonString =
        R"({"valueObject": {"intField": 1, "boolField": true, "stringField": "stringValue"},
"valuesObject": {"intsField": [1, 2, 3], "boolsField": [true, false, true], "stringsField": ["stringValue1", "stringValue2", "stringValue3"]}})";
    const ExtendedCpp::Json json = ExtendedCpp::Json::parse(jsonString);

    // Act
    const auto [valueObject, valuesObject] = json.get<ObjectObject>();

    // Assert
    ASSERT_EQ(valueObject->intField, 1);
    ASSERT_EQ(valueObject->boolField, true);
    ASSERT_EQ(valueObject->stringField, "stringValue");

    ASSERT_EQ(valuesObject->intsField[0], 1);
    ASSERT_EQ(valuesObject->boolsField[0], true);
    ASSERT_EQ(valuesObject->stringsField[0], "stringValue1");

    ASSERT_EQ(valuesObject->intsField[1], 2);
    ASSERT_EQ(valuesObject->boolsField[1], false);
    ASSERT_EQ(valuesObject->stringsField[1], "stringValue2");

    ASSERT_EQ(valuesObject->intsField[2], 3);
    ASSERT_EQ(valuesObject->boolsField[2], true);
    ASSERT_EQ(valuesObject->stringsField[2], "stringValue3");
}

TEST(JsonTests, OBJECTS_FROM_JSON_Test)
{
    // Average
    const std::string jsonString =
        R"({"valueObjects": [{"intField": 1, "boolField": true, "stringField": "stringValue1"},
{"intField": 2, "boolField": false, "stringField": "stringValue2"},
{"intField": 3, "boolField": true, "stringField": "stringValue3"}]})";
    const ExtendedCpp::Json json = ExtendedCpp::Json::parse(jsonString);

    // Act
    const auto [valueObjects] = json.get<ObjectsObject>();

    // Assert
    ASSERT_EQ(valueObjects[0]->intField, 1);
    ASSERT_EQ(valueObjects[0]->boolField, true);
    ASSERT_EQ(valueObjects[0]->stringField, "stringValue1");

    ASSERT_EQ(valueObjects[1]->intField, 2);
    ASSERT_EQ(valueObjects[1]->boolField, false);
    ASSERT_EQ(valueObjects[1]->stringField, "stringValue2");

    ASSERT_EQ(valueObjects[2]->intField, 3);
    ASSERT_EQ(valueObjects[2]->boolField, true);
    ASSERT_EQ(valueObjects[2]->stringField, "stringValue3");
}

TEST(JsonTests, VALUE_TO_JSON_Test)
{
    // Average
    ValueObject valueObject;
    valueObject.intField = 1;
    valueObject.boolField = true;
    valueObject.stringField = "stringValue1";

    // Act
    const ExtendedCpp::Json json = std::make_shared<ValueObject>(valueObject);
    const auto [intField, boolField, stringField] = json.get<ValueObject>();

    // Assert
    ASSERT_FALSE(json.dump().empty());

    ASSERT_EQ(intField, valueObject.intField);
    ASSERT_EQ(boolField, valueObject.boolField);
    ASSERT_EQ(stringField, valueObject.stringField);
}

TEST(JsonTests, VALUES_TO_JSON_Test)
{
    // Average
    ValuesObject valuesObject;
    valuesObject.intsField = { 1, 2, 3 };
    valuesObject.boolsField = { true, false, true };
    valuesObject.stringsField = { "stringValue1", "stringValue2", "stringValue3" };

    // Act
    const ExtendedCpp::Json json = std::make_shared<ValuesObject>(valuesObject);
    const auto [intsField, boolsField, stringsField] = json.get<ValuesObject>();

    // Assert
    ASSERT_FALSE(json.dump().empty());

    ASSERT_EQ(intsField[0], valuesObject.intsField[0]);
    ASSERT_EQ(boolsField[0], valuesObject.boolsField[0]);
    ASSERT_EQ(stringsField[0], valuesObject.stringsField[0]);

    ASSERT_EQ(intsField[1], valuesObject.intsField[1]);
    ASSERT_EQ(boolsField[1], valuesObject.boolsField[1]);
    ASSERT_EQ(stringsField[1], valuesObject.stringsField[1]);

    ASSERT_EQ(intsField[2], valuesObject.intsField[2]);
    ASSERT_EQ(boolsField[2], valuesObject.boolsField[2]);
    ASSERT_EQ(stringsField[2], valuesObject.stringsField[2]);
}

TEST(JsonTests, OBJECT_TO_JSON_Test)
{
    // Average
    ValueObject valueObject;
    valueObject.intField = 1;
    valueObject.boolField = true;
    valueObject.stringField = "stringValue1";

    ValuesObject valuesObject;
    valuesObject.intsField = { 1, 2, 3 };
    valuesObject.boolsField = { true, false, true };
    valuesObject.stringsField = { "stringValue1", "stringValue2", "stringValue3" };

    ObjectObject objectObject;
    objectObject.valueObject = std::make_shared<ValueObject>(valueObject);
    objectObject.valuesObject = std::make_shared<ValuesObject>(valuesObject);

    // Act
    const ExtendedCpp::Json json = std::make_shared<ObjectObject>(objectObject);
    const auto assertObject = json.get<ObjectObject>();

    // Assert
    ASSERT_FALSE(json.dump().empty());

    ASSERT_EQ(assertObject.valueObject->intField, objectObject.valueObject->intField);
    ASSERT_EQ(assertObject.valueObject->boolField, objectObject.valueObject->boolField);
    ASSERT_EQ(assertObject.valueObject->stringField, objectObject.valueObject->stringField);

    ASSERT_EQ(assertObject.valuesObject->intsField[0], objectObject.valuesObject->intsField[0]);
    ASSERT_EQ(assertObject.valuesObject->boolsField[0], objectObject.valuesObject->boolsField[0]);
    ASSERT_EQ(assertObject.valuesObject->stringsField[0], objectObject.valuesObject->stringsField[0]);

    ASSERT_EQ(assertObject.valuesObject->intsField[1], objectObject.valuesObject->intsField[1]);
    ASSERT_EQ(assertObject.valuesObject->boolsField[1], objectObject.valuesObject->boolsField[1]);
    ASSERT_EQ(assertObject.valuesObject->stringsField[1], objectObject.valuesObject->stringsField[1]);

    ASSERT_EQ(assertObject.valuesObject->intsField[2], objectObject.valuesObject->intsField[2]);
    ASSERT_EQ(assertObject.valuesObject->boolsField[2], objectObject.valuesObject->boolsField[2]);
    ASSERT_EQ(assertObject.valuesObject->stringsField[2], objectObject.valuesObject->stringsField[2]);
}

TEST(JsonTests, OBJECTS_TO_JSON_Test)
{
    // Average
    ValueObject valueObject1;
    valueObject1.intField = 1;
    valueObject1.boolField = true;
    valueObject1.stringField = "stringValue1";

    ValueObject valueObject2;
    valueObject2.intField = 2;
    valueObject2.boolField = false;
    valueObject2.stringField = "stringValue2";

    ValueObject valueObject3;
    valueObject3.intField = 3;
    valueObject3.boolField = true;
    valueObject3.stringField = "stringValue3";

    ObjectsObject objectsObject;
    objectsObject.valueObjects = { std::make_shared<ValueObject>(valueObject1),
        std::make_shared<ValueObject>(valueObject2),
        std::make_shared<ValueObject>(valueObject3) };

    // Act
    const ExtendedCpp::Json json = std::make_shared<ObjectsObject>(objectsObject);
    const auto [valueObjects] = json.get<ObjectsObject>();

    // Assert
    ASSERT_FALSE(json.dump().empty());

    ASSERT_EQ(valueObjects[0]->intField, objectsObject.valueObjects[0]->intField);
    ASSERT_EQ(valueObjects[0]->boolField, objectsObject.valueObjects[0]->boolField);
    ASSERT_EQ(valueObjects[0]->stringField, objectsObject.valueObjects[0]->stringField);

    ASSERT_EQ(valueObjects[1]->intField, objectsObject.valueObjects[1]->intField);
    ASSERT_EQ(valueObjects[1]->boolField, objectsObject.valueObjects[1]->boolField);
    ASSERT_EQ(valueObjects[1]->stringField, objectsObject.valueObjects[1]->stringField);

    ASSERT_EQ(valueObjects[2]->intField, objectsObject.valueObjects[2]->intField);
    ASSERT_EQ(valueObjects[2]->boolField, objectsObject.valueObjects[2]->boolField);
    ASSERT_EQ(valueObjects[2]->stringField, objectsObject.valueObjects[2]->stringField);
}