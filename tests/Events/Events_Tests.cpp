#include <gtest/gtest.h>

#include <ExtendedCpp/Events/Event.h>
#include <ExtendedCpp/Events/FunctionHandler.h>
#include <ExtendedCpp/Events/MethodHandler.h>
#include <ExtendedCpp/Events/StaticMethodHandler.h>
#include <ExtendedCpp/Events/ConstMethodHandler.h>

#include "Events_Tests.h"

TEST(EventsTests, FunectionHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();
    const std::function functor1 = [](const EventType::Ptr&){};
    const std::function functor2 = [](const EventType::Ptr&){};

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateFunctionHandler(functor1);
    const auto handler2 = FUNCTION_HANDLER(functor2);

    // Assert
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
}

TEST(EventsTests, MethodHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();
    HandlerClass handlerClass;

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateMethodHandler(&handlerClass, &HandlerClass::Handler);
    const auto handler2 = METHOD_HANDLER(&handlerClass, &HandlerClass::Handler);
    const auto handler3 = CONST_METHOD_HANDLER(&handlerClass, &HandlerClass::ConstHandler);

    // Assert
    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
    ASSERT_FALSE(event.Contains(handler3));
    
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);
    ASSERT_NO_THROW(event += handler3);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));
    ASSERT_TRUE(event.Contains(handler3));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);
    ASSERT_NO_THROW(event -= handler3);

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
    ASSERT_FALSE(event.Contains(handler3));
}

TEST(EventsTests, StaticMethodHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateStaticMethodHandler(&HandlerClass::StaticHandler);
    const auto handler2 = STATIC_METHOD_HANDLER(&HandlerClass::StaticHandler);

    // Assert
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
}