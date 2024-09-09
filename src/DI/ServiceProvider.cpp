#include <DI/ServiceProvider.h>

DI::ServiceProvider::ServiceProvider(const ServiceProvider& serviceProvider) noexcept
{
    _factories = serviceProvider._factories;
    _instances = serviceProvider._instances;
}

DI::ServiceProvider::ServiceProvider(ServiceProvider&& serviceProvider) noexcept
{
    _factories = std::move(serviceProvider._factories);
    _instances = std::move(serviceProvider._instances);
}

DI::ServiceProvider& DI::ServiceProvider::operator=(ServiceProvider&& serviceProvider) noexcept
{
    _factories = std::move(serviceProvider._factories);
    _instances = std::move(serviceProvider._instances);
    return *this;
}

std::shared_ptr<void> DI::ServiceProvider::GetService(std::type_index typeIndex) const noexcept
{
    if (!_factories.contains(typeIndex))
        return { nullptr };

    try
    {
        return GetServiceImplementation(typeIndex);
    }
    catch (...)
    {
        return { nullptr };
    }
}

std::shared_ptr<void> DI::ServiceProvider::GetServiceRequired(std::type_index typeIndex) const
{
    if (!_factories.contains(typeIndex))
        throw std::invalid_argument("Service " + std::string(typeIndex.name()) + " does not found!");
    return GetServiceImplementation(typeIndex);
}

std::shared_ptr<void> DI::ServiceProvider::GetServiceImplementation(std::type_index typeIndex) const
{
    const auto& [factory, lifeTime] = _factories.at(typeIndex);

    switch (lifeTime)
    {
        case LifeTime::Singleton:
            if (!_instances.contains(typeIndex))
                _instances.insert(std::pair<std::type_index, std::shared_ptr<void>>(typeIndex, factory(*this)));
            return _instances.at(typeIndex);
        case LifeTime::Transient:
            return factory(*this);
        case LifeTime::Scoped:
            if (_instances.contains(typeIndex) && _instances.at(typeIndex).use_count() == 1)
                _instances.erase(_instances.find(typeIndex));
            if (!_instances.contains(typeIndex))
                _instances.insert(std::pair<std::type_index, std::shared_ptr<void>>(typeIndex, factory(*this)));
            return _instances.at(typeIndex);
    }

    throw std::invalid_argument("Service " + std::string(typeIndex.name()) + " does not found!");
}