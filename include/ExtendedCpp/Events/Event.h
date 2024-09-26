#ifndef Events_Event_H
#define Events_Event_H

#include <memory>
#include <list>
#include <shared_mutex>
#include <mutex>

#include <ExtendedCpp/Events/IEventHandler.h>

namespace ExtendedCpp::Events
{
    template<typename ...TParams>
    class Event final
    {
    private:
        using EventHandler = std::shared_ptr<IEventHandler<TParams...>>;

        std::list<EventHandler> _handlers;
        mutable std::shared_mutex _listMutex;

    public:
        Event() noexcept = default;
        ~Event() = default;

        void operator()(TParams... params) const
        {
            std::shared_lock lock(_listMutex);
            for (const auto& handler : _handlers)
                handler->Call(std::forward<TParams>(params)...);
        }

        void operator+=(const EventHandler& handler) noexcept
        {
            if (!Contains(handler))
            {
                std::unique_lock lock(_listMutex);
                _handlers.push_back(handler);
            }
        }

        void operator-=(const EventHandler& handler) noexcept
        {
            if (Contains(handler))
            {
                std::unique_lock lock(_listMutex);
                _handlers.erase(Find(handler));
            }
        }

        bool Contains(const EventHandler& handler) const noexcept
        {
            std::shared_lock lock(_listMutex);
            for (const EventHandler& element : _handlers)
                if (*handler == *element) return true;
            return false;
        }

    private:
        auto Find(const EventHandler& handler) const noexcept
        {
            for (auto iterator = _handlers.begin(); iterator != _handlers.end(); ++iterator)
                if (**iterator == *handler)
                    return iterator;
            return _handlers.end();
        }
    };
}

#endif