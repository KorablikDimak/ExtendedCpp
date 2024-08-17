#include <gtest/gtest.h>

#include "TestStruct.h"

TEST(ReflectionTests, GetMetaInfoTest)
{
    TestStruct testStruct;

    auto members = testStruct.meta.GetMembers();
    auto fields = testStruct.meta.GetFields();
    auto methods = testStruct.meta.GetMethods();

    auto testFields = testStruct.meta.GetField("IntField");
    int* fieldPtr = (int*) testFields->Value();
    *fieldPtr = 5;

    auto testMethods = testStruct.meta.GetMethods("TestMethodInt");
    auto result = std::any_cast<int>(testMethods[0]->Invoke());
}