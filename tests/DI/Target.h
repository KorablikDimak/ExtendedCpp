#ifndef TARGET_H
#define TARGET_H

#include <memory>

#include <ExtendedCpp/Reflection.h>

#include "IService.h"

class Target
{
private:
    std::shared_ptr<IService> _service;

public:
    explicit Target(const std::shared_ptr<IService>& service) noexcept : _service(service) {}

    META(Target,
         CONSTRUCTOR(std::shared_ptr<IService>))
};

#endif