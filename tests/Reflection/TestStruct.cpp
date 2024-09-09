#include "TestStruct.h"

TestStruct::TestStruct(const std::string& str) noexcept
{
    StringField = str;
}

int TestStruct::TestMethodInt() const noexcept
{
    return IntField;
}

double TestStruct::TestMethodDouble(double a, double b) const noexcept
{
    return IntField + a + b;
}

double TestStruct::TestMethodDouble(double a, double b, double c) const noexcept
{
    return IntField + a + b + c;
}

std::string TestStruct::TestMethodStaticString() noexcept
{
    return "test";
}

int TestStruct::TestMethodStatic() noexcept
{
    return 10;
}

int TestStruct::TestMethodStatic(int a) noexcept
{
    return 10 + a;
}