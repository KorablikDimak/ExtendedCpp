#ifndef TestTemplate_H
#define TestTemplate_H

#include <Reflection/Reflection.h>

template<typename T>
struct TestTemplate
{
    T field;

    TestTemplate() noexcept = default;

    T GetT() noexcept
    {
        return field;
    }

    void SetT(T value) noexcept
    {
        field = value;
    }

    META_TEMPLATE(TestTemplate,
                  CONSTRUCTOR(),
                  FIELD(field),
                  METHOD(GetT),
                  METHOD(SetT, T))
};

#endif