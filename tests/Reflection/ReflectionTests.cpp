#include <gtest/gtest.h>

#include "TestStruct.h"

TEST(ReflectionTests, MetaFieldTest)
{
    // Average
    TestStruct testStruct;

    // Act
    int* fieldPtr = std::any_cast<int*>(TestStruct::MetaInfo.GetField("IntField")->GetField(&testStruct));
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
    auto result = std::any_cast<int>(TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke(&testStruct));

    // Assert
    ASSERT_EQ(result, 7);
    ASSERT_TRUE(TestStruct::MetaInfo.GetMethods("TestMethodDouble")[0]->Parameters()[0] == typeid(double));
}

TEST(ReflectionTests, MetaConstructorTest)
{
    // Average
    TestStruct testStruct;

    // Act
    auto createdStruct = std::any_cast<TestStruct>(TestStruct::MetaInfo.GetConstructors()[0]->Create());
    auto newStruct = std::static_pointer_cast<TestStruct>(TestStruct::MetaInfo.GetConstructors()[0]->New());

    // Assert
    ASSERT_TRUE(newStruct.get() != nullptr);
}

TEST(ReflectionTests, GetTypeTest)
{
    // Average
    auto typeInfo1 = Reflection::GetType<TestStruct>();
    TestStruct testStruct;
    auto typeInfo2 = Reflection::GetType(testStruct);

    // Act
    auto staticField = std::any_cast<double*>(typeInfo1.GetStaticFields()[0]->GetField());
    *staticField = 6;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());
}