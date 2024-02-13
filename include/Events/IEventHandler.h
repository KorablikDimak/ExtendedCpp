#ifndef Events_EventHandler_H
#define Events_EventHandler_H

namespace Events
{
    template<typename ...TParams>
    class IEventHandler
    {
    public:
        IEventHandler() = default;
        virtual ~IEventHandler() = default;

        bool operator==(const IEventHandler& other) const
        {
            return IsEquals(other);
        }

        bool operator!=(const IEventHandler& other) const
        {
            return !IsEquals(other);
        }

        virtual void Call(TParams... params) = 0;

    protected:
        virtual bool IsEquals(const IEventHandler& other) const = 0;
    };
}

#endif