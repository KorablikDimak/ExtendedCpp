#ifndef Events_Tests_H
#define Events_Tests_H

class EventType final
{
public:
    typedef std::shared_ptr<EventType> Ptr;
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