#include <gtest/gtest.h>

#include <Events/Event.h>
#include <Events/FunctionHandler.h>
#include <Events/MethodHandler.h>
#include <Events/StaticMethodHandler.h>

#include "Events_Tests.h"

TEST(EventsTests, FunectionHandlerTest)
{
    // Average
    auto event = Events::Event<const EventType::Ptr&>();
    const std::function functor1 = [](const EventType::Ptr&){};
    const std::function functor2 = [](const EventType::Ptr&){};

    // Act
    const auto handler1 = Events::CreateFunctionHandler(functor1);
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
    auto event = Events::Event<const EventType::Ptr&>();
    HandlerClass handlerClass;

    // Act
    const auto handler1 = Events::CreateMethodHandler(&handlerClass, &HandlerClass::Handler);
    const auto handler2 = METHOD_HANDLER(&handlerClass, &HandlerClass::Handler);

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

TEST(EventsTests, StaticMethodHandlerTest)
{
    // Average
    auto event = Events::Event<const EventType::Ptr&>();

    // Act
    const auto handler1 = Events::CreateStaticMethodHandler(&HandlerClass::StaticHandler);
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