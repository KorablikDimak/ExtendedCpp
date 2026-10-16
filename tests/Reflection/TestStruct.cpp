#include "TestStruct.h"

#include <utility>

TestStruct::TestStruct(std::string  str) noexcept : StringField(std::move(str)) {}

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

std::string_view TestStruct::TestMethodStaticString() noexcept
{
    return "test";
}

int TestStruct::TestMethodStatic() noexcept
{
    return 10; // NOLINT(*-magic-numbers)
}

int TestStruct::TestMethodStatic(const int a) noexcept
{
    return 10 + a; // NOLINT(*-magic-numbers)
}