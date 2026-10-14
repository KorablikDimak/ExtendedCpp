#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H

#include <ExtendedCpp/Reflection.h>

struct TestStruct
{
    int IntField;
    std::string Name;

    META(TestStruct,
        FIELD(IntField),
        FIELD(Name))
};

#endif