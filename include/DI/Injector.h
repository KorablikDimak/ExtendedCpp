#ifndef DI_Injector_H
#define DI_Injector_H

#include <DI/ServiceProvider.h>

namespace DI
{
    template<typename TTarget>
    class Injector
    {
    public:
        Injector() noexcept = delete;

        static std::optional<TTarget> Create(const ServiceProvider& serviceProvider) noexcept
        {
            auto typeInfo = Reflection::GetType<TTarget>();

            for (const std::shared_ptr<Reflection::ConstructorInfo>& constructor : typeInfo.GetConstructors())
            {
                std::vector<std::shared_ptr<void>> args;
                for (const std::type_index& param : constructor->Parameters())
                {
                    std::shared_ptr<void> service = serviceProvider.GetService(param);
                    if (!service) break;
                    args.push_back(std::move(service));
                }

                if (args.size() != constructor->Parameters().size()) continue;

                try
                {
                    return constructor->CreateFromAny<TTarget>(args);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }

            return std::nullopt;
        }

        static TTarget CreateRequired(const ServiceProvider& serviceProvider)
        {
            auto typeInfo = Reflection::GetType<TTarget>();

            for (const std::shared_ptr<Reflection::ConstructorInfo>& constructor : typeInfo.GetConstructors())
            {
                std::vector<std::shared_ptr<void>> args;
                for (const std::type_index& param : constructor->Parameters())
                {
                    std::shared_ptr<void> service = serviceProvider.GetService(param);
                    if (!service) break;
                    args.push_back(std::move(service));
                }

                if (args.size() != constructor->Parameters().size()) continue;
                return constructor->CreateFromAny<TTarget>(args);
            }

            throw std::runtime_error("Can not create type " + std::string(typeid(TTarget).name()));
        }

        static std::shared_ptr<TTarget> Ptr(const ServiceProvider& serviceProvider) noexcept
        {
            auto typeInfo = Reflection::GetType<TTarget>();

            for (const std::shared_ptr<Reflection::ConstructorInfo>& constructor : typeInfo.GetConstructors())
            {
                std::vector<std::shared_ptr<void>> args;
                for (const std::type_index& param : constructor->Parameters())
                {
                    std::shared_ptr<void> service = serviceProvider.GetService(param);
                    if (!service) break;
                    args.push_back(std::move(service));
                }

                if (args.size() != constructor->Parameters().size()) continue;

                try
                {
                    return constructor->NewFromAny<TTarget>(args);
                }
                catch (...)
                {
                    return { nullptr };
                }
            }

            return { nullptr };
        }

        static std::shared_ptr<TTarget> PtrRequired(const ServiceProvider& serviceProvider)
        {
            auto typeInfo = Reflection::GetType<TTarget>();

            for (const std::shared_ptr<Reflection::ConstructorInfo>& constructor : typeInfo.GetConstructors())
            {
                std::vector<std::shared_ptr<void>> args;
                for (const std::type_index& param : constructor->Parameters())
                {
                    std::shared_ptr<void> service = serviceProvider.GetService(param);
                    if (!service) break;
                    args.push_back(std::move(service));
                }

                if (args.size() != constructor->Parameters().size()) continue;
                return constructor->NewFromAny<TTarget>(args);
            }

            throw std::runtime_error("Can not create type " + std::string(typeid(TTarget).name()));
        }
    };
}

#endif