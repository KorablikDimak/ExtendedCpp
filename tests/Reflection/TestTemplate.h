#ifndef TestTemplate_H
#define TestTemplate_H

#include <ExtendedCpp/Reflection.h>

template<typename T>
struct TestTemplate
{
    T field;

    TestTemplate() noexcept = default;

private:
    T GetT() noexcept
    {
        return field;
    }

    void SetT(T value) noexcept
    {
        field = value;
    }

public:
    META(TestTemplate,
         CONSTRUCTOR(),
         FIELD(field),
         METHOD(GetT),
         METHOD(SetT, T))
};

#endif