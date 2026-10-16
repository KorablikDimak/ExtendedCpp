#ifndef DI_CONCEPTS_H
#define DI_CONCEPTS_H

#include <memory>
#include <utility>

/// @brief
namespace ExtendedCpp::DI::Concepts
{
    template<typename TTarget, typename... TDependencies>
    concept ConstructableFromSharedPtr = requires
    {
        TTarget(std::declval<std::shared_ptr<TDependencies>>()...);
    };
}

#endif