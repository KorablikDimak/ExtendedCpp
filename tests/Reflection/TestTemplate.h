#ifndef TEST_TEMPLATE_H
#define TEST_TEMPLATE_H

#include <ExtendedCpp/Reflection.h>

template<typename T>
struct TestTemplate
{
    T Field;
    std::vector<T> Data;

    TestTemplate() noexcept = default;

private:
    T GetT() noexcept
    {
        return Field;
    }

    void SetT(T value) noexcept
    {
        Field = value;
    }

public:
    META(TestTemplate,
         CONSTRUCTOR(),
         FIELD(Field),
         FIELD(Data),
         METHOD(GetT),
         METHOD(SetT, T))
};

#endif