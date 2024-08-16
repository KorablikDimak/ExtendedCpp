#include <gtest/gtest.h>

#include "Reflection/Reflection.h"

using namespace Common;

struct TestStruct
{
    int IntField{};
    std::string StringField;
    int IntPtrField{};

    META(FIELD(IntField), FIELD(StringField), FIELD(IntPtrField))
};

TEST(ReflectionTests, GetFieldTest)
{
    TestStruct testStruct;

    auto fieldMeta = testStruct.Meta.GetFieldMeta<0>();
    using FieldType = decltype(fieldMeta)::Type;

    FieldType& fieldRef = fieldMeta.Field();
    const FieldType& fieldRefConst = fieldMeta.Field();
    FieldType field = fieldMeta.Field();

    auto& autoFieldRef = fieldMeta.Field();
    const auto& autoFieldRefConst = fieldMeta.Field();
    auto autoField = fieldMeta.Field();

    const std::string& fieldNameRef = fieldMeta.Name();
    std::string fieldName = fieldMeta.Name();

    testStruct.Meta.ForeachFields([](const auto& fieldMeta){ ASSERT_FALSE(fieldMeta.Name().empty()); });
}