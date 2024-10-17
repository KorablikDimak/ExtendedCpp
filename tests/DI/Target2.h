#ifndef Target2_H
#define Target2_H

#include <memory>

#include <ExtendedCpp/Reflection.h>

#include "IService.h"
#include "IService2.h"

class Target2
{
private:
    std::shared_ptr<IService> _service;
    std::shared_ptr<IService2> _service2;

public:
    explicit Target2(const std::shared_ptr<IService>& service, const std::shared_ptr<IService2>& service2)
    {
        _service = service;
        _service2 = service2;
    }

    META(Target2,
         CONSTRUCTOR(std::shared_ptr<IService>, std::shared_ptr<IService2>))
};

#endif