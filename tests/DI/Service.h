#ifndef SERVICE_H
#define SERVICE_H

#include "IService.h"

class Service final : public IService
{
public:
    Service() noexcept = default;
    ~Service() noexcept override = default;
};

#endif