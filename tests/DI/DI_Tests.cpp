#include <gtest/gtest.h>

#include <DI/Register.h>
#include <DI/Injector.h>

#include "Service.h"
#include "Target.h"

TEST(DI_Tests, DI_ServiceProviderTest1) {
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();
    serviceProvider.AddTransient<IService, Service>();
    serviceProvider.AddScoped<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>());
}

TEST(DI_Tests, DI_ServiceProviderTest2)
{
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService>();
    serviceProvider.AddTransient<IService>();
    serviceProvider.AddScoped<IService>();

    // Assert
    ASSERT_NO_THROW(auto service = serviceProvider.GetService<IService>();
    ASSERT_TRUE(service));

    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>();
    ASSERT_TRUE(service));
}

TEST(DI_Tests, DI_ServiceProviderTest3)
{
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService>([](const DI::ServiceProvider&){ return std::make_shared<Service>(); });
    serviceProvider.AddTransient<IService>([](const DI::ServiceProvider&){ return std::make_shared<Service>(); });
    serviceProvider.AddScoped<IService>([](const DI::ServiceProvider&){ return std::make_shared<Service>(); });

    // Assert
    ASSERT_NO_THROW(auto service = serviceProvider.GetService<IService>();
    ASSERT_TRUE(service));

    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>();
    ASSERT_TRUE(service));
}

TEST(DI_Tests, DI_RegisterTest)
{
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::Create(serviceProvider);
    ASSERT_TRUE(target.has_value()));
    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::CreateRequired(serviceProvider));

    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::New(serviceProvider);
    ASSERT_TRUE(target));
    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::NewRequired(serviceProvider);
    ASSERT_TRUE(target));

    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::Ptr(serviceProvider);
    ASSERT_TRUE(target));
    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::PtrRequired(serviceProvider);
    ASSERT_TRUE(target));
}

TEST(DI_Tests, DI_InjectorTest)
{
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto target = DI::Injector<Target>::Create(serviceProvider);
    ASSERT_TRUE(target.has_value()));
    ASSERT_NO_THROW(auto target = DI::Injector<Target>::CreateRequired(serviceProvider));

    ASSERT_NO_THROW(auto target = DI::Injector<Target>::Ptr(serviceProvider);
    ASSERT_TRUE(target));
    ASSERT_NO_THROW(auto target = DI::Injector<Target>::PtrRequired(serviceProvider);
    ASSERT_TRUE(target));
}