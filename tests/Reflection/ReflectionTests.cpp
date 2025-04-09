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
    auto result1 = std::any_cast<int>(TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke(&testStruct));
    auto result2 = TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke<int>(&testStruct);

    // Assert
    ASSERT_EQ(result1, 6);
    ASSERT_EQ(result2, 5);
    ASSERT_TRUE(TestStruct::MetaInfo.GetMethods("TestMethodDouble")[0]->Parameters()[0] == typeid(double));
}

TEST(ReflectionTests, ConstMetaMethodTest)
{
    // Average
    const TestStruct testStruct;

    // Act
    auto result1 = std::any_cast<int>(TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke(&testStruct));
    auto result2 = TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke<int>(&testStruct);

    // Assert
    ASSERT_EQ(result1, 0);
    ASSERT_EQ(result2, 0);
    ASSERT_TRUE(TestStruct::MetaInfo.GetMethods("TestMethodDouble")[0]->Parameters()[0] == typeid(double));
}

TEST(ReflectionTests, MetaStaticMethodTest)
{
    // Average
    auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();

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
    auto typeInfo1 = ExtendedCpp::Reflection::GetType<TestStruct>();
    TestStruct testStruct;
    auto typeInfo2 = ExtendedCpp::Reflection::GetType(typeid(testStruct));

    // Act
    auto staticField1 = std::any_cast<double*>(typeInfo1.GetStaticFields()[0]->GetField());
    *staticField1 = 6;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField1) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());

    // Act
    auto staticField2 = typeInfo1.GetStaticFields()[0]->GetField<double>();
    *staticField2 = 9;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField2) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());
}

TEST(ReflectionTests, ConstructorFromAnyTest)
{
    // Average
    auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();
    auto constructors = typeInfo.GetConstructors();

    // Act
    auto testStruct1 = constructors[1]->CreateFromAny({std::string("test")});
    auto testStruct2 = constructors[1]->NewFromAny({std::string("test")});

    // Assert
    ASSERT_TRUE(testStruct1.has_value());
    ASSERT_TRUE(testStruct2);
    ASSERT_NO_THROW([[maybe_unused]] auto _ = std::any_cast<TestStruct>(testStruct1));
    ASSERT_NO_THROW([[maybe_unused]] auto _= std::static_pointer_cast<TestStruct>(testStruct2));
}

TEST(ReflectionTests, MetaTemplateTest)
{
    // Average
    auto typeInfo1 = TestTemplate<int>::MetaInfo;
    auto typeInfo2 = TestTemplate<double>::MetaInfo;
    auto typeInfo3 = TestTemplate<std::string>::MetaInfo;

    // Act
    // Assert
    ASSERT_NO_THROW([[maybe_unused]] auto _ =
            std::any_cast<TestTemplate<int>>(typeInfo1.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW([[maybe_unused]] auto _ =
            std::any_cast<TestTemplate<double>>(typeInfo2.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW([[maybe_unused]] auto _ =
            std::any_cast<TestTemplate<std::string>>(typeInfo3.GetConstructors()[0]->Create()));
}

TEST(ReflectionTests, GetCollectionInfoTest)
{
    // Average
    auto typeInfo = TestTemplate<int>::MetaInfo;

    // Act
    const auto fieldInfo = typeInfo.GetField("data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Assert
    ASSERT_TRUE(fieldInfo);
    ASSERT_TRUE(fieldInfo->IsCollection());
    ASSERT_TRUE(collectionFieldInfo);
}

TEST(ReflectionTests, GetElementTest)
{
    // Average
    auto typeInfo = TestTemplate<int>::MetaInfo;
    const auto fieldInfo = typeInfo.GetField("data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Act
    TestTemplate<int> object;
    object.data = { 1, 2, 3, 4 };
    const int element0 = collectionFieldInfo->GetElementValue<int>(&object, 0);
    const int element1 = collectionFieldInfo->GetElementValue<int>(&object, 1);
    const int element2 = collectionFieldInfo->GetElementValue<int>(&object, 2);
    const int element3 = collectionFieldInfo->GetElementValue<int>(&object, 3);

    // Assert
    ASSERT_EQ(element0, object.data[0]);
    ASSERT_EQ(element1, object.data[1]);
    ASSERT_EQ(element2, object.data[2]);
    ASSERT_EQ(element3, object.data[3]);
}

TEST(ReflectionTests, InsertionFieldTest)
{
    // Average
    auto typeInfo = TestTemplate<int>::MetaInfo;
    const auto fieldInfo = typeInfo.GetField("data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Act
    TestTemplate<int> object;
    collectionFieldInfo->InsertBack(&object, 1);
    collectionFieldInfo->InsertBack(&object, 3);
    collectionFieldInfo->InsertBack(&object, 6);
    collectionFieldInfo->InsertBack(&object, 9);

    // Assert
    ASSERT_EQ(object.data.size(), 4);
    ASSERT_EQ(1, object.data[0]);
    ASSERT_EQ(3, object.data[1]);
    ASSERT_EQ(6, object.data[2]);
    ASSERT_EQ(9, object.data[3]);
}