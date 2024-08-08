#ifndef Events_Event_H
#define Events_Event_H

#include <memory>
#include <list>
#include <shared_mutex>

#include <Events/IEventHandler.h>

namespace Events
{
    template<typename ...TParams>
    class Event final
    {
    private:
        using EventHandler = std::shared_ptr<IEventHandler<TParams...>>;
        using EventHandlerIterator = typename std::list<EventHandler>::iterator;

        std::list<EventHandler> _handlers;
        mutable std::shared_mutex _listMutex;
        EventHandlerIterator _currentIterator;
        bool _currentRemoved = false;

    public:
        Event() = default;
        ~Event() = default;

        void operator()(TParams... params)
        {
            _listMutex.lock_shared();
            _currentIterator = _handlers.begin();
            while (_currentIterator != _handlers.end())
            {
                _currentRemoved = false;
                _listMutex.unlock_shared();
                (*_currentIterator)->Call(params...);
                _listMutex.lock_shared();
                if (!_currentRemoved)
                    ++_currentIterator;
            }
            _listMutex.unlock_shared();
        }

        void operator+=(const EventHandler& handler)
        {
            std::lock_guard lock(_listMutex);
            if (!Contains(handler))
                _handlers.push_back(handler);
        }

        void operator-=(const EventHandler& handler)
        {
            std::lock_guard lock(_listMutex);
            auto iterator = Find(handler);
            if (iterator != _handlers.end())
            {
                if (_currentIterator == iterator)
                {
                    _currentRemoved = true;
                    _currentIterator = _handlers.erase(iterator);
                }
                else
                    _handlers.erase(iterator);
            }
        }

        bool Contains(const EventHandler& handler) const
        {
            for (const EventHandler& element : _handlers)
                if (*handler == *element) return true;
            return false;
        }

    private:
        auto Find(const EventHandler& handler) const
        {
            for (auto iterator = _handlers.begin(); iterator != _handlers.end(); ++iterator)
                if (**iterator == *handler) return iterator;
            return _handlers.end();
        }
    };
}

#endif