#ifndef DI_Concepts_H
#define DI_Concepts_H

#include <utility>
#include <memory>

namespace ExtendedCpp::DI::Concepts
{
    template<typename TTarget, typename... TDependencies>
    concept ConstructableFromSharedPtr = requires
    {
        TTarget(std::declval<std::shared_ptr<TDependencies...>>());
    };
}

#endif