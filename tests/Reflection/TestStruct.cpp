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
