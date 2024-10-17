#ifndef Service2_H
#define Service2_H

#include "IService2.h"

class Service2 final : public IService2
{
public:
    Service2() = default;
    ~Service2() override = default;
};

#endif