#ifndef DI_Concepts_H
#define DI_Concepts_H

#include <Reflection/TypeTraits.h>

namespace DI::Concepts
{
    template<typename TTarget, typename... TDependencies>
    concept ConstructableFromSharedPtr = requires
    {
        TTarget(std::declval<std::shared_ptr<TDependencies...>>());
    };
}

#endif