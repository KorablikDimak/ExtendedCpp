#include <gtest/gtest.h>

#include <ExtendedCpp/Events.h>

#include "Events_Tests.h"

void Handler(const EventType::Ptr&) noexcept {}

TEST(EventsTests, FunctionHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();

    // Act
    const auto handler1 = FUNCTION_HANDLER(&InlineHandler);
    const auto handler2 = FUNCTION_HANDLER(&Handler);
    const auto& handler3 = handler2;

    // Assert
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);
    ASSERT_NO_THROW(event += handler3);

    ASSERT_TRUE(event.size() == 2);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
}

TEST(EventsTests, FunctorHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();
    auto functor1 = [](const EventType::Ptr&){};
    std::function functor2 = [](const EventType::Ptr&){};

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateFunctorHandler(functor1);
    const auto handler2 = FUNCTOR_HANDLER(functor2);

    // Assert
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);

    ASSERT_TRUE(event.size() == 2);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);

    ASSERT_TRUE(event.empty());

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
}

TEST(EventsTests, MethodHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();
    HandlerClass handlerClass;

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateMethodHandler(&handlerClass, &HandlerClass::Handler1);
    const auto handler2 = METHOD_HANDLER(&handlerClass, &HandlerClass::Handler2);
    const auto handler3 = CONST_METHOD_HANDLER(&handlerClass, &HandlerClass::ConstHandler);

    // Assert
    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
    ASSERT_FALSE(event.Contains(handler3));
    
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);
    ASSERT_NO_THROW(event += handler3);

    ASSERT_TRUE(event.size() == 3);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));
    ASSERT_TRUE(event.Contains(handler3));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);
    ASSERT_NO_THROW(event -= handler3);

    ASSERT_TRUE(event.empty());

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
    ASSERT_FALSE(event.Contains(handler3));
}

TEST(EventsTests, StaticMethodHandlerTest)
{
    // Average
    auto event = ExtendedCpp::Events::Event<const EventType::Ptr&>();

    // Act
    const auto handler1 = ExtendedCpp::Events::CreateStaticMethodHandler(&HandlerClass::StaticHandler1);
    const auto handler2 = STATIC_METHOD_HANDLER(&HandlerClass::StaticHandler2);

    // Assert
    ASSERT_NO_THROW(event += handler1);
    ASSERT_NO_THROW(event += handler2);
    ASSERT_NO_THROW(event += handler2);

    ASSERT_TRUE(event.size() == 2);

    ASSERT_TRUE(event.Contains(handler1));
    ASSERT_TRUE(event.Contains(handler2));

    ASSERT_NO_THROW(event -= handler1);
    ASSERT_NO_THROW(event -= handler2);

    ASSERT_TRUE(event.empty());

    ASSERT_FALSE(event.Contains(handler1));
    ASSERT_FALSE(event.Contains(handler2));
}