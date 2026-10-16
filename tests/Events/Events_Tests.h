#ifndef EVENTS_TESTS_H
#define EVENTS_TESTS_H

// ReSharper disable CppMemberFunctionMayBeStatic

class EventType final
{
public:
    using Ptr = std::shared_ptr<EventType>;
};

class HandlerClass final
{
public:
    void Handler1(const EventType::Ptr&) noexcept {}
    void Handler2(const EventType::Ptr&) noexcept {}
    void ConstHandler(const EventType::Ptr&) const noexcept {}
    static void StaticHandler1(const EventType::Ptr&) noexcept {}
    static void StaticHandler2(const EventType::Ptr&) noexcept {}
};

inline void InlineHandler(const EventType::Ptr&) noexcept {}

#endif