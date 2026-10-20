#include <gtest/gtest.h>

#include "TestStruct.h"
#include "TestTemplate.h"

// NOLINTBEGIN(*-magic-numbers)

TEST(ReflectionTests, MetaFieldTest)
{
    // Average
    TestStruct testStruct;

    // Act
    testStruct.IntField = 0;
    TestStruct::StaticDoubleField = 0;

    auto *const fieldPtr =
            std::any_cast<int*>(TestStruct::MetaInfo.GetField("IntField")->GetPtr(&testStruct));
    *fieldPtr = 5;

    auto *const staticFieldPtr =
            std::any_cast<double*>(TestStruct::MetaInfo.GetStaticField("StaticDoubleField")->GetPtr());
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
    const auto result1 = std::any_cast<int>(TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke(&testStruct));
    const auto result2 = TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke<int>(&testStruct);

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
    const auto result1 = std::any_cast<int>(TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke(&testStruct));
    const auto result2 = TestStruct::MetaInfo.GetMethods("TestMethodInt")[0]->Invoke<int>(&testStruct);

    // Assert
    ASSERT_EQ(result1, 0);
    ASSERT_EQ(result2, 0);
    ASSERT_TRUE(TestStruct::MetaInfo.GetMethods("TestMethodDouble")[0]->Parameters()[0] == typeid(double));
}

TEST(ReflectionTests, MetaStaticMethodTest)
{
    // Average
    const auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();

    // Act
    const auto staticMethodInfo = typeInfo.GetStaticMethods();

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
    [[maybe_unused]] const TestStruct testStruct;

    // Act
    [[maybe_unused]] const auto createdStruct = std::any_cast<TestStruct>(TestStruct::MetaInfo.GetConstructors()[0]->Create());
    const auto newStruct = std::static_pointer_cast<TestStruct>(TestStruct::MetaInfo.GetConstructors()[0]->New());

    // Assert
    ASSERT_TRUE(newStruct.get() != nullptr);
}

TEST(ReflectionTests, GetTypeTest)
{
    // Average
    const auto typeInfo1 = ExtendedCpp::Reflection::GetType<TestStruct>();
    const auto typeInfo2 = ExtendedCpp::Reflection::GetType(typeid(TestStruct));

    // Act
    auto *const staticField1 = std::any_cast<double*>(typeInfo1.GetStaticFields()[0]->GetPtr());
    *staticField1 = 6;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField1) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());

    // Act
    auto *const staticField2 = typeInfo1.GetStaticFields()[0]->GetPtr<double>();
    *staticField2 = 9;

    // Assert
    ASSERT_TRUE(std::abs(TestStruct::StaticDoubleField - *staticField2) < 0.001);
    ASSERT_TRUE(typeInfo2.has_value());
}

TEST(ReflectionTests, ConstructorFromAnyTest)
{
    // Average
    const auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();
    const auto constructors = typeInfo.GetConstructors();

    // Act
    const auto testStruct1 = constructors[1]->Create({std::string("test")});
    const auto testStruct2 = constructors[1]->New({std::string("test")});

    // Assert
    ASSERT_TRUE(testStruct1.has_value());
    ASSERT_TRUE(testStruct2);
    ASSERT_NO_THROW([[maybe_unused]] auto _ = std::any_cast<TestStruct>(testStruct1)); // NOLINT(*-avoid-goto)
    ASSERT_NO_THROW([[maybe_unused]] auto _ = std::static_pointer_cast<TestStruct>(testStruct2)); // NOLINT(*-avoid-goto)
}

TEST(ReflectionTests, MetaTemplateTest)
{
    // Average
    auto typeInfo1 = TestTemplate<int>::MetaInfo;
    auto typeInfo2 = TestTemplate<double>::MetaInfo;
    auto typeInfo3 = TestTemplate<std::string>::MetaInfo;

    // Act
    // Assert
    ASSERT_NO_THROW([[maybe_unused]] auto _ = // NOLINT(*-avoid-goto)
            std::any_cast<TestTemplate<int>>(typeInfo1.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW([[maybe_unused]] auto _ = // NOLINT(*-avoid-goto)
            std::any_cast<TestTemplate<double>>(typeInfo2.GetConstructors()[0]->Create()));
    ASSERT_NO_THROW([[maybe_unused]] auto _ = // NOLINT(*-avoid-goto)
            std::any_cast<TestTemplate<std::string>>(typeInfo3.GetConstructors()[0]->Create()));
}

TEST(ReflectionTests, GetCollectionInfoTest)
{
    // Average
    const auto typeInfo = TestTemplate<int>::MetaInfo;

    // Act
    const auto fieldInfo = typeInfo.GetField("Data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Assert
    ASSERT_TRUE(fieldInfo);
    ASSERT_TRUE(fieldInfo->IsCollection());
    ASSERT_TRUE(collectionFieldInfo);
}

TEST(ReflectionTests, GetElementTest)
{
    // Average
    const auto typeInfo = TestTemplate<int>::MetaInfo;
    const auto fieldInfo = typeInfo.GetField("Data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Act
    TestTemplate<int> object;
    object.Data = { 1, 2, 3, 4 };
    const int element0 = collectionFieldInfo->GetElementValue<int>(&object, 0);
    const int element1 = collectionFieldInfo->GetElementValue<int>(&object, 1);
    const int element2 = collectionFieldInfo->GetElementValue<int>(&object, 2);
    const int element3 = collectionFieldInfo->GetElementValue<int>(&object, 3);

    // Assert
    ASSERT_EQ(element0, object.Data[0]);
    ASSERT_EQ(element1, object.Data[1]);
    ASSERT_EQ(element2, object.Data[2]);
    ASSERT_EQ(element3, object.Data[3]);
}

TEST(ReflectionTests, InsertionFieldTest)
{
    // Average
    const auto typeInfo = TestTemplate<int>::MetaInfo;
    const auto fieldInfo = typeInfo.GetField("Data");
    const auto collectionFieldInfo = std::dynamic_pointer_cast<ExtendedCpp::Reflection::CollectionFieldInfo>(fieldInfo);

    // Act
    TestTemplate<int> object;
    collectionFieldInfo->InsertBack(&object, 1);
    collectionFieldInfo->InsertBack(&object, 3);
    collectionFieldInfo->InsertBack(&object, 6);
    collectionFieldInfo->InsertBack(&object, 9);

    // Assert
    ASSERT_EQ(object.Data.size(), 4);
    ASSERT_EQ(1, object.Data[0]);
    ASSERT_EQ(3, object.Data[1]);
    ASSERT_EQ(6, object.Data[2]);
    ASSERT_EQ(9, object.Data[3]);
}

TEST(ReflectionTests, SetValueTest1)
{
    // Average
    const auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();

    // Act
    TestStruct object{};
    const auto fieldInfo = typeInfo.GetField("IntField");
    ASSERT_EQ(0, object.IntField);
    fieldInfo->SetValue(&object, 5);

    // Asser
    ASSERT_EQ(5, object.IntField);
}

TEST(ReflectionTests, SetValueTest2)
{
    // Average
    const auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();

    // Act
    TestStruct object{};
    const auto fieldInfo = typeInfo.GetField("StringField");
    // ReSharper disable once CppVariableCanBeMadeConstexpr
    const std::string value = "TestString";
    ASSERT_TRUE(object.StringField.empty());
    fieldInfo->SetValue(&object, value);

    // Asser
    ASSERT_TRUE(value == object.StringField);
}

TEST(ReflectionTests, CreateAndSetValueTest)
{
    const auto typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();

    // Act
    auto object = typeInfo.GetDefaultConstructor()->New();
    ASSERT_EQ(0, std::static_pointer_cast<TestStruct>(object)->IntField);
    const auto fieldInfo = typeInfo.GetField("IntField");
    fieldInfo->SetValue(object, 7);

    // Asser
    ASSERT_EQ(7, std::static_pointer_cast<TestStruct>(object)->IntField);
}

// NOLINTEND(*-magic-numbers)