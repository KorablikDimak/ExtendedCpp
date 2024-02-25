#include <gtest/gtest.h>

#include <DI/Register.h>

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
    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>());
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
    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>());
}

TEST(DI_Tests, DI_RegisterTest)
{
    // Average
    DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto target = DI::Register<Target(IService)>::CreateRequired(serviceProvider));
}