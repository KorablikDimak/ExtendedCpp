#include <gtest/gtest.h>

#include <ExtendedCpp/DI/Register.h>
#include <ExtendedCpp/DI/Injector.h>

#include "Service.h"
#include "Target.h"

TEST(DI_Tests, DI_ServiceProviderTest1) {
    // Average
    ExtendedCpp::DI::ServiceProvider serviceProvider;

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
    ExtendedCpp::DI::ServiceProvider serviceProvider;

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
    ExtendedCpp::DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService>([](const ExtendedCpp::DI::ServiceProvider&){ return std::make_shared<Service>(); });
    serviceProvider.AddTransient<IService>([](const ExtendedCpp::DI::ServiceProvider&){ return std::make_shared<Service>(); });
    serviceProvider.AddScoped<IService>([](const ExtendedCpp::DI::ServiceProvider&){ return std::make_shared<Service>(); });

    // Assert
    ASSERT_NO_THROW(auto service = serviceProvider.GetService<IService>();
    ASSERT_TRUE(service));

    ASSERT_NO_THROW(auto service = serviceProvider.GetServiceRequired<IService>();
    ASSERT_TRUE(service));
}

TEST(DI_Tests, DI_RegisterTest)
{
    // Average
    ExtendedCpp::DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::Create(serviceProvider);
    ASSERT_TRUE(target.has_value()));
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::CreateRequired(serviceProvider));

    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::New(serviceProvider);
    ASSERT_TRUE(target);
    delete target);
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::NewRequired(serviceProvider);
    ASSERT_TRUE(target);
    delete target);

    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::Ptr(serviceProvider);
    ASSERT_TRUE(target));
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Register<Target(IService)>::PtrRequired(serviceProvider);
    ASSERT_TRUE(target));
}

TEST(DI_Tests, DI_InjectorTest)
{
    // Average
    ExtendedCpp::DI::ServiceProvider serviceProvider;

    // Act
    serviceProvider.AddSingleton<IService, Service>();

    // Assert
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Injector<Target>::Create(serviceProvider);
    ASSERT_TRUE(target.has_value()));
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Injector<Target>::CreateRequired(serviceProvider));

    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Injector<Target>::Ptr(serviceProvider);
    ASSERT_TRUE(target));
    ASSERT_NO_THROW(auto target = ExtendedCpp::DI::Injector<Target>::PtrRequired(serviceProvider);
    ASSERT_TRUE(target));
}