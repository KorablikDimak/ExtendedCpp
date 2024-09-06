#include <gtest/gtest.h>

#include "TestStruct.h"
#include "TestTemplate.h"

TEST(ReflectionTests, MetaFieldTest)
{
    // Average
    TestStruct testStruct;

    // Act
    testStruct.IntField = 0;
    TestStruct::StaticDoubleField = 0;

    auto fieldPtr =
            std::any_cast<int*>(TestStruct::MetaInfo.GetField("IntField")->GetField(&testStruct));
    *fieldPtr = 5;

    auto staticFieldPtr =
            std::any_cast<double*>(TestStruct::MetaInfo.GetStaticField("StaticDoubleField")->GetField());
    *staticFieldPtr = 8;

    // Assert
    ASSERT_EQ(testStruct.IntField, 5);
    ASSERT_EQ(testStruct.StaticDoubleField, 8);
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

TEST(ReflectionTests, MetaStaticMethodTest)
{
    // Average
    auto typeInfo = Reflection::GetType<TestStruct>();

    // Act
    auto staticMethodInfo = typeInfo.GetStaticMethods();

    // Assert
    ASSERT_FALSE(staticMethodInfo.empty());
    ASSERT_TRUE(staticMethodInfo.size() == 3);

    ASSERT_TRUE(staticMethodInfo[0]->Name() == "TestMethodStaticString");
    ASSERT_TRUE(staticMethodInfo[1]->Name() == "TestMethodStatic");
    ASSERT_TRUE(staticMethodInfo[2]->Name() == "TestMethodStatic");

    ASSERT_TRUE(staticMethodInfo[0]->Parameters().empty());
    ASSERT_TRUE(staticMethodInfo[1]->Parameters().empty());
    ASSERT_TRUE(staticMethodInfo[2]->Parameters().size() == 1);
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
    auto typeInfo2 = Reflection::GetType(typeid(testStruct));

    // Act
    auto staticField = std::any_cast<double*>(typeInfo1.GetStaticFields()[0]->GetField());
    *staticField = 6;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());
}

TEST(ReflectionTests, ConstructorFromAnyTest)
{
    // Average
    auto typeInfo = Reflection::GetType<TestStruct>();
    auto constructors = typeInfo.GetConstructors();

    // Act
    auto testStruct1 = constructors[1]->CreateFromAny({std::string("test")});
    auto testStruct2 = constructors[1]->NewFromAny({std::string("test")});

    // Assert
    ASSERT_TRUE(testStruct1.has_value());
    ASSERT_TRUE(testStruct2);
    ASSERT_NO_THROW(std::any_cast<TestStruct>(testStruct1));
    ASSERT_NO_THROW(std::static_pointer_cast<TestStruct>(testStruct2));
}

TEST(ReflectionTests, MetaTemplateTest)
{
    // Average
    auto typeInfo1 = TestTemplate<int>::MetaInfo;
    auto typeInfo2 = TestTemplate<double>::MetaInfo;
    auto typeInfo3 = TestTemplate<std::string>::MetaInfo;

    // Act
    // Assert
    ASSERT_NO_THROW(auto t1 =
            std::any_cast<TestTemplate<int>>(typeInfo1.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW(auto t2 =
            std::any_cast<TestTemplate<double>>(typeInfo2.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW(auto t3 =
            std::any_cast<TestTemplate<std::string>>(typeInfo3.GetConstructors()[0]->Create()));
}