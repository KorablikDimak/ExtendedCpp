#ifndef DI_LifeTime_H
#define DI_LifeTime_H

namespace ExtendedCpp::DI
{
    /// @brief 
    enum class LifeTime
    {
        Singleton,
        Transient,
        Scoped
    };
}

#endif