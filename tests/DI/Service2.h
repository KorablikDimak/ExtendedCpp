#ifndef SERVICE2_H
#define SERVICE2_H

#include "IService2.h"

class Service2 final : public IService2
{
public:
    Service2() noexcept = default;
    ~Service2() noexcept override = default;
};

#endif