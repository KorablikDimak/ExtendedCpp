#ifndef TestStruct_H
#define TestStruct_H

#include <Reflection/Reflection.h>

struct TestStruct
{
    int IntField{};
    std::string StringField;
    int* IntPtrField{};

    inline static double StaticDoubleField{};

    TestStruct() noexcept = default;
    explicit TestStruct(const std::string& str) noexcept;

private:
    [[nodiscard]]
    int TestMethodInt() const noexcept;
    [[nodiscard]]
    double TestMethodDouble(double a, double b) const noexcept;
    [[nodiscard]]
    double TestMethodDouble(double a, double b, double c) const noexcept;

    static std::string TestMethodStaticString() noexcept;
    static int TestMethodStatic() noexcept;
    static int TestMethodStatic(int a) noexcept;

public:
    META(TestStruct,
         FIELD(IntField),
         FIELD(StringField),
         FIELD(IntPtrField),
         STATIC_FIELD(StaticDoubleField),
         CONSTRUCTOR(),
         CONSTRUCTOR(std::string),
         METHOD(TestMethodInt),
         METHOD(TestMethodDouble, double, double),
         METHOD(TestMethodDouble, double, double, double),
         STATIC_METHOD(TestMethodStaticString),
         STATIC_METHOD(TestMethodStatic),
         STATIC_METHOD(TestMethodStatic, int));
};

#endif