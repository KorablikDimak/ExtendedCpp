#ifndef DI_ServiceProvider_H
#define DI_ServiceProvider_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include <DI/LifeTime.h>

namespace DI
{
    class ServiceProvider final
    {
    private:
        std::map<std::type_index, std::pair<std::any, LifeTime>> _factories;
        mutable std::map<std::type_index, std::any> _instances;

    public:
        template<typename TService, typename TImplementation>
        void AddSingleton()
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddSingleton<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService, typename TImplementation>
        void AddTransient()
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddTransient<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService, typename TImplementation>
        void AddScoped()
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddScoped<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService>
        void AddSingleton()
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddSingleton<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddTransient()
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddTransient<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddScoped()
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddScoped<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddSingleton(std::function<std::shared_ptr<TService>(const ServiceProvider&)> serviceFactory)
        {
            if (_factories.contains(typeid(TService)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(TService));
                factory = serviceFactory;
                lifeTime = LifeTime::Singleton;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<std::any, LifeTime>>
                    (typeid(TService), std::make_pair(serviceFactory, LifeTime::Singleton)));
            }
        }

        template<typename TService>
        void AddTransient(std::function<std::shared_ptr<TService>(const ServiceProvider&)> serviceFactory)
        {
            if (_factories.contains(typeid(TService)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(TService));
                factory = serviceFactory;
                lifeTime = LifeTime::Transient;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<std::any, LifeTime>>
                    (typeid(TService), std::make_pair(serviceFactory, LifeTime::Transient)));
            }
        }

        template<typename TService>
        void AddScoped(std::function<std::shared_ptr<TService>(const ServiceProvider&)> serviceFactory)
        {
            if (_factories.contains(typeid(TService)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(TService));
                factory = serviceFactory;
                lifeTime = LifeTime::Scoped;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<std::any, LifeTime>>
                    (typeid(TService), std::make_pair(serviceFactory, LifeTime::Scoped)));
            }
        }

        template<typename TService>
        std::shared_ptr<TService> GetService() const
        {
            if (!_factories.contains(typeid(TService)))
                return {};
            return GetServiceImplementation<TService>();
        }

        template<typename TService>
        std::shared_ptr<TService> GetServiceRequired() const
        {
            if (!_factories.contains(typeid(TService)))
                throw std::domain_error("Service " + std::string(typeid(TService).name()) + " does not found!");
            return GetServiceImplementation<TService>();
        }

    private:
        template<typename TService>
        std::shared_ptr<TService> GetServiceImplementation() const
        {
            const auto [any, lifeTime] = _factories.at(typeid(TService));
            auto factory = std::any_cast<std::function<std::shared_ptr<TService>(const ServiceProvider&)>>(any);

            switch (lifeTime)
            {
                case LifeTime::Singleton:
                    if (!_instances.contains(typeid(TService)))
                        _instances.insert(std::make_pair<std::type_index, std::any>(typeid(TService), factory(*this)));
                    return std::any_cast<std::shared_ptr<TService>>(_instances.at(typeid(TService)));
                case LifeTime::Transient:
                    return factory(*this);
                case LifeTime::Scoped:
                    if (_instances.contains(typeid(TService)) && std::any_cast<std::shared_ptr<TService>>(_instances.at(typeid(TService))).use_count() == 1)
                        _instances.erase(_instances.find(typeid(TService)));
                    if (!_instances.contains(typeid(TService)))
                        _instances.insert(std::make_pair<std::type_index, std::any>(typeid(TService), factory(*this)));
                    return std::any_cast<std::shared_ptr<TService>>(_instances.at(typeid(TService)));
            }
            return nullptr;
        }
    };
}

#endif