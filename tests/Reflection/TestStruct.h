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

    static std::string TestMethodStaticString() noexcept;
    static int TestMethodStatic() noexcept;
    static int TestMethodStatic(int a) noexcept;

    META_DECL;
};

#endif