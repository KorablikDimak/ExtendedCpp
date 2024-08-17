#include <gtest/gtest.h>

#include "TestStruct.h"

TEST(ReflectionTests, GetMetaInfoTest)
{
    TestStruct testStruct;

    auto members = testStruct.MetaInfo.GetMembers();
    auto fields = testStruct.MetaInfo.GetFields();
    auto methods = testStruct.MetaInfo.GetMethods();
    auto constructors = testStruct.MetaInfo.GetConstructors();

    auto constructor = std::any_cast<TestStruct>(constructors[0]->Create());

    auto testFields = testStruct.MetaInfo.GetField("IntField");
    int* fieldPtr = (int*) testFields->Value();
    *fieldPtr = 5;

    auto testMethods = testStruct.MetaInfo.GetMethods("TestMethodInt");
    auto result = std::any_cast<int>(testMethods[0]->Invoke());
}