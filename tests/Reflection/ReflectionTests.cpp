#include <gtest/gtest.h>

#include "TestStruct.h"

TEST(ReflectionTests, MetaFieldTest)
{
    // Average
    TestStruct testStruct;

    // Act
    int* fieldPtr = (int*) testStruct.MetaInfo.GetField("IntField")->Value();
    *fieldPtr = 5;

    // Assert
    ASSERT_EQ(testStruct.IntField, 5);
}

TEST(ReflectionTests, MetaMethodTest)
{
    // Average
    TestStruct testStruct;
    testStruct.IntField = 7;

    // Act
    auto result = std::any_cast<int>(testStruct.MetaInfo.GetMethods("TestMethodInt")[0]->Invoke());

    // Assert
    ASSERT_EQ(result, 7);
    ASSERT_TRUE(testStruct.MetaInfo.GetMethods("TestMethodDouble")[0]->Parameters()[0] == typeid(double));
}

TEST(ReflectionTests, MetaConstructorTest)
{
    // Average
    TestStruct testStruct;

    // Act
    auto createdStruct = std::any_cast<TestStruct>(testStruct.MetaInfo.GetConstructors()[0]->Create());
    auto newStruct = std::any_cast<TestStruct*>(testStruct.MetaInfo.GetConstructors()[0]->New());

    // Assert
    ASSERT_TRUE(newStruct != nullptr);
    delete newStruct;
    newStruct = nullptr;
}