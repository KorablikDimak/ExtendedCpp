#ifndef TestStruct_H
#define TestStruct_H

#include <Reflection/Reflection.h>

using namespace Reflection;

struct TestStruct
{
    int IntField{};
    std::string StringField;
    int IntPtrField{};

    TestStruct() noexcept = default;
    explicit TestStruct(const std::string& str) noexcept;

    [[nodiscard]]
    int TestMethodInt() const noexcept;
    [[nodiscard]]
    double TestMethodDouble(double a, double b) const noexcept;

    META(CONSTRUCTOR(), CONSTRUCTOR(const std::string&),
         FIELD(IntField), FIELD(StringField), FIELD(IntPtrField),
         METHOD(TestMethodInt), METHOD(TestMethodDouble, double, double))
};

#endif