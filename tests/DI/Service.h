#ifndef Service_H
#define Service_H

#include "IService.h"

class Service final : public IService
{
public:
    Service() = default;
    ~Service() override = default;
};

#endif