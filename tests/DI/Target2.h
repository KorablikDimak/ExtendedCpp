#ifndef TARGET2_H
#define TARGET2_H

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
    explicit Target2(const std::shared_ptr<IService>& service, const std::shared_ptr<IService2>& service2) noexcept :
        _service(service), _service2(service2) {}

    META(Target2,
         CONSTRUCTOR(std::shared_ptr<IService>, std::shared_ptr<IService2>))
};

#endif