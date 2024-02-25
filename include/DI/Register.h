#ifndef DI_Register_H
#define DI_Register_H

#include <DI/ServiceProvider.h>

namespace DI
{
    template<typename TTarget>
    class Register;

    template<typename TTarget, typename... TDependencies>
    class Register<TTarget(TDependencies...)>
    {
    private:
        template<std::size_t Index = 0>
        static auto CreateDependencyTuple(const ServiceProvider& provider, auto dependencyTuple)
        {
            if constexpr (Index == sizeof...(TDependencies))
                return dependencyTuple;
            else
            {
                using TDependency = std::tuple_element_t<Index, std::tuple<TDependencies...>>;
                auto dependency = provider.GetService<TDependency>();
                return CreateDependencyTuple<Index + 1>(provider, std::tuple_cat(std::move(dependencyTuple), std::make_tuple(std::move(dependency))));
            }
        }

        template<std::size_t Index = 0>
        static auto CreateDependencyTupleRequired(const ServiceProvider& provider, auto dependencyTuple)
        {
            if constexpr (Index == sizeof...(TDependencies))
                return dependencyTuple;
            else
            {
                using TDependency = std::tuple_element_t<Index, std::tuple<TDependencies...>>;
                auto dependency = provider.GetServiceRequired<TDependency>();
                return CreateDependencyTupleRequired<Index + 1>(provider, std::tuple_cat(std::move(dependencyTuple), std::make_tuple(std::move(dependency))));
            }
        }

    public:
        static auto Create(const ServiceProvider& provider)
        {
            auto dependencyTuple = CreateDependencyTuple(provider, std::make_tuple());
            return CallConstructor(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
        }

        static auto CreateRequired(const ServiceProvider& provider)
        {
            auto dependencyTuple = CreateDependencyTupleRequired(provider, std::make_tuple());
            return CallConstructor(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
        }

    private:
        template<std::size_t ...Indexes>
        static auto CallConstructor(std::index_sequence<Indexes...>, auto&& dependencyTuple)
        {
            return TTarget(std::get<Indexes>(dependencyTuple)...);
        }
    };
}

#endif