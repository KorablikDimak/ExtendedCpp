#ifndef Events_Tests_H
#define Events_Tests_H

class EventType
{
public:
    typedef std::shared_ptr<EventType> Ptr;
};

class HandlerClass
{
public:
    void Handler(const EventType::Ptr&) {}
    static void StaticHandler(const EventType::Ptr&) {}
};

#endif