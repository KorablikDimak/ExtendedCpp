#ifndef DI_ServiceProvider_H
#define DI_ServiceProvider_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include <DI/LifeTime.h>
#include <Reflection/Reflection.h>

namespace DI
{
    class ServiceProvider final
    {
    private:
        using FactoryType = std::function<std::shared_ptr<void>(const ServiceProvider&)>;

        std::map<std::type_index, std::pair<FactoryType, LifeTime>> _factories;
        mutable std::map<std::type_index, std::shared_ptr<void>> _instances;

    public:
        ServiceProvider() noexcept = default;
        ServiceProvider(const ServiceProvider& serviceProvider) noexcept;
        ServiceProvider(ServiceProvider&& serviceProvider) noexcept;

        ~ServiceProvider() = default;

        ServiceProvider& operator=(const ServiceProvider& serviceProvider) noexcept = default;
        ServiceProvider& operator=(ServiceProvider&& serviceProvider) noexcept;

        template<typename TService, typename TImplementation>
        void AddSingleton() noexcept
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddSingleton<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService, typename TImplementation>
        void AddTransient() noexcept
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddTransient<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService, typename TImplementation>
        void AddScoped() noexcept
        {
            static_assert(std::is_default_constructible<TImplementation>() == true);
            AddScoped<TService>([](const ServiceProvider&)
                { return std::make_shared<TImplementation>(); });
        }

        template<typename TService>
        void AddSingleton() noexcept
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddSingleton<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddTransient() noexcept
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddTransient<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddScoped() noexcept
        {
            static_assert(std::is_default_constructible<TService>() == true);
            AddScoped<TService>([](const ServiceProvider&)
                { return std::make_shared<TService>(); });
        }

        template<typename TService>
        void AddSingleton(const std::function<std::shared_ptr<void>(const ServiceProvider&)>& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = serviceFactory;
                lifeTime = LifeTime::Singleton;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Singleton)));
            }
        }

        template<typename TService>
        void AddSingleton(std::function<std::shared_ptr<void>(const ServiceProvider&)>&& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = std::move(serviceFactory);
                lifeTime = LifeTime::Singleton;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Singleton)));
            }
        }

        template<typename TService>
        void AddTransient(const std::function<std::shared_ptr<void>(const ServiceProvider&)>& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = serviceFactory;
                lifeTime = LifeTime::Transient;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Transient)));
            }
        }

        template<typename TService>
        void AddTransient(std::function<std::shared_ptr<void>(const ServiceProvider&)>&& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = std::move(serviceFactory);
                lifeTime = LifeTime::Transient;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Transient)));
            }
        }

        template<typename TService>
        void AddScoped(const std::function<std::shared_ptr<void>(const ServiceProvider&)>& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = serviceFactory;
                lifeTime = LifeTime::Scoped;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Scoped)));
            }
        }

        template<typename TService>
        void AddScoped(std::function<std::shared_ptr<void>(const ServiceProvider&)>&& serviceFactory) noexcept
        {
            if (_factories.contains(typeid(std::shared_ptr<TService>)))
            {
                auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));
                factory = std::move(serviceFactory);
                lifeTime = LifeTime::Scoped;
            }
            else
            {
                _factories.insert(std::make_pair<std::type_index, std::pair<FactoryType, LifeTime>>
                    (typeid(std::shared_ptr<TService>), std::make_pair(serviceFactory, LifeTime::Scoped)));
            }
        }

        template<typename TService>
        std::shared_ptr<TService> GetService() const noexcept
        {
            if (!_factories.contains(typeid(std::shared_ptr<TService>)))
                return { nullptr };

            try
            {
                return GetServiceImplementation<TService>();
            }
            catch (...)
            {
                return { nullptr };
            }
        }

        template<typename TService>
        std::shared_ptr<TService> GetServiceRequired() const
        {
            if (!_factories.contains(typeid(std::shared_ptr<TService>)))
                throw std::invalid_argument("Service " + std::string(typeid(std::shared_ptr<TService>).name()) + " does not found!");
            return GetServiceImplementation<TService>();
        }

        std::shared_ptr<void> GetService(std::type_index typeIndex) const noexcept;
        std::shared_ptr<void> GetServiceRequired(std::type_index typeIndex) const;

    private:
        template<typename TService>
        std::shared_ptr<TService> GetServiceImplementation() const
        {
            const auto& [factory, lifeTime] = _factories.at(typeid(std::shared_ptr<TService>));

            switch (lifeTime)
            {
                case LifeTime::Singleton:
                    if (!_instances.contains(typeid(std::shared_ptr<TService>)))
                        _instances.insert(std::make_pair<std::type_index, std::shared_ptr<void>>(typeid(std::shared_ptr<TService>), factory(*this)));
                    return std::static_pointer_cast<TService>(_instances.at(typeid(std::shared_ptr<TService>)));
                case LifeTime::Transient:
                    return std::static_pointer_cast<TService>(factory(*this));
                case LifeTime::Scoped:
                    if (_instances.contains(typeid(std::shared_ptr<TService>)) && _instances.at(typeid(std::shared_ptr<TService>)).use_count() == 1)
                        _instances.erase(_instances.find(typeid(std::shared_ptr<TService>)));
                    if (!_instances.contains(typeid(std::shared_ptr<TService>)))
                        _instances.insert(std::make_pair<std::type_index, std::shared_ptr<void>>(typeid(std::shared_ptr<TService>), factory(*this)));
                    return std::static_pointer_cast<TService>(_instances.at(typeid(std::shared_ptr<TService>)));
            }
        }

        std::shared_ptr<void> GetServiceImplementation(std::type_index typeIndex) const;
    };
}

#endif