#ifndef TestStruct_H
#define TestStruct_H

#include <Reflection/Reflection.h>

using namespace Reflection;

struct TestStruct
{
    int IntField{};
    std::string StringField;
    int* IntPtrField{};

    inline static double StaticDoubleField{};

    TestStruct() noexcept = default;
    explicit TestStruct(const std::string& str) noexcept;

    [[nodiscard]]
    int TestMethodInt() const noexcept;
    [[nodiscard]]
    double TestMethodDouble(double a, double b) const noexcept;
    [[nodiscard]]
    double TestMethodDouble(double a, double b, double c) const noexcept;

    static int TestMethodStatic() noexcept;
    static int TestMethodStatic(int a) noexcept;

    META(FIELD(IntField),
         FIELD(StringField),
         FIELD(IntPtrField),
         STATIC_FIELD(StaticDoubleField),
         CONSTRUCTOR(),
         CONSTRUCTOR(const std::string&),
         METHOD(TestMethodInt),
         METHOD(TestMethodDouble, double, double),
         METHOD(TestMethodDouble, double, double, double),
         STATIC_METHOD(TestMethodStatic),
         STATIC_METHOD(TestMethodStatic, int))
};

#endif