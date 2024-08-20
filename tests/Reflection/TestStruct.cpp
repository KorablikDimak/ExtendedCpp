#include "TestStruct.h"

META_IMPL(TestStruct,
          FIELD(IntField),
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

int TestStruct::TestMethodStatic() noexcept
{
    return 10;
}

int TestStruct::TestMethodStatic(int a) noexcept
{
    return 10 + a;
}
