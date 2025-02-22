#ifndef DI_Register_H
#define DI_Register_H

#include <optional>
#include <tuple>

#include <ExtendedCpp/DI/ServiceProvider.h>
#include <ExtendedCpp/DI/Concepts.h>

/// @brief 
namespace ExtendedCpp::DI
{
    /// @brief 
    /// @tparam TTarget 
    template<typename TTarget>
    class Register;

    /// @brief 
    /// @tparam TTarget 
    /// @tparam ...TDependencies 
    template<typename TTarget, typename... TDependencies>
    requires Concepts::ConstructableFromSharedPtr<TTarget, TDependencies...>
    class Register<TTarget(TDependencies...)>
    {
    private:
        template<std::size_t Index = 0>
        static auto CreateDependencyTuple(const ServiceProvider& provider, auto dependencyTuple) noexcept
        {
            if constexpr (Index == sizeof...(TDependencies))
                return dependencyTuple;
            else
            {
                using TDependency = std::tuple_element_t<Index, std::tuple<TDependencies...>>;
                auto dependency = provider.GetService<TDependency>();
                return CreateDependencyTuple<Index + 1>(
                        provider, std::tuple_cat(std::move(dependencyTuple), std::make_tuple(std::move(dependency))));
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
                return CreateDependencyTupleRequired<Index + 1>(
                        provider, std::tuple_cat(std::move(dependencyTuple), std::make_tuple(std::move(dependency))));
            }
        }

    public:
        /// @brief 
        Register() noexcept = delete;

        /// @brief 
        /// @param provider 
        /// @return 
        static std::optional<TTarget> Create(const ServiceProvider& provider) noexcept
        {
            try
            {
                auto dependencyTuple = CreateDependencyTuple(provider, std::make_tuple());
                return CallConstructor(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
            }
            catch (...)
            {
                return std::nullopt;
            }
        }

        /// @brief 
        /// @param provider 
        /// @return 
        static TTarget CreateRequired(const ServiceProvider& provider)
        {
            auto dependencyTuple = CreateDependencyTupleRequired(provider, std::make_tuple());
            return CallConstructor(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
        }

        /// @brief 
        /// @param provider 
        /// @return 
        static TTarget* New(const ServiceProvider& provider) noexcept
        {
            try
            {
                auto dependencyTuple = CreateDependencyTuple(provider, std::make_tuple());
                return CallConstructorNew(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
            }
            catch (...)
            {
                return nullptr;
            }
        }

        /// @brief 
        /// @param provider 
        /// @return 
        static TTarget* NewRequired(const ServiceProvider& provider)
        {
            auto dependencyTuple = CreateDependencyTupleRequired(provider, std::make_tuple());
            return CallConstructorNew(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
        }

        /// @brief 
        /// @param provider 
        /// @return 
        static std::shared_ptr<TTarget> Ptr(const ServiceProvider& provider) noexcept
        {
            try
            {
                auto dependencyTuple = CreateDependencyTuple(provider, std::make_tuple());
                return CallConstructorPtr(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
            }
            catch (...)
            {
                return { nullptr };
            }
        }

        /// @brief 
        /// @param provider 
        /// @return 
        static std::shared_ptr<TTarget> PtrRequired(const ServiceProvider& provider)
        {
            auto dependencyTuple = CreateDependencyTupleRequired(provider, std::make_tuple());
            return CallConstructorPtr(std::make_index_sequence<sizeof...(TDependencies)>{}, std::move(dependencyTuple));
        }

    private:
        template<std::size_t... Indexes>
        static TTarget CallConstructor(std::index_sequence<Indexes...>, auto&& dependencyTuple)
        {
            return TTarget(std::get<Indexes>(dependencyTuple)...);
        }

        template<std::size_t... Indexes>
        static TTarget* CallConstructorNew(std::index_sequence<Indexes...>, auto&& dependencyTuple)
        {
            return new TTarget(std::get<Indexes>(dependencyTuple)...);
        }

        template<std::size_t... Indexes>
        static std::shared_ptr<TTarget> CallConstructorPtr(std::index_sequence<Indexes...>, auto&& dependencyTuple)
        {
            return std::make_shared<TTarget>(std::get<Indexes>(dependencyTuple)...);
        }
    };
}

#endif