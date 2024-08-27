#ifndef Events_EventHandler_H
#define Events_EventHandler_H

namespace Events
{
    template<typename ...TParams>
    class IEventHandler
    {
    public:
        IEventHandler() noexcept = default;
        virtual ~IEventHandler() = default;

        bool operator==(const IEventHandler& other) const noexcept
        {
            return IsEquals(other);
        }

        bool operator!=(const IEventHandler& other) const noexcept
        {
            return !IsEquals(other);
        }

        virtual void Call(TParams... params) = 0;

    protected:
        virtual bool IsEquals(const IEventHandler& other) const noexcept = 0;
    };
}

#endif