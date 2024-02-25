#ifndef DI_LifeTime_H
#define DI_LifeTime_H

namespace DI
{
    enum class LifeTime
    {
        Singleton,
        Transient,
        Scoped
    };
}

#endif