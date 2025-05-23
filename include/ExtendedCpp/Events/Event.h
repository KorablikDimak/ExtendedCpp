#ifndef Events_Event_H
#define Events_Event_H

#include <memory>
#include <list>
#include <shared_mutex>
#include <mutex>

#include <ExtendedCpp/Events/IEventHandler.h>

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam ...TParams 
    template<typename ...TParams>
    class Event final
    {
    private:
        using EventHandler = std::shared_ptr<IEventHandler<TParams...>>;

        std::list<EventHandler> _handlers;
        mutable std::shared_mutex _listMutex;

    public:
        /// @brief 
        Event() noexcept = default;

        /// @brief 
        ~Event() = default;

        /// @brief 
        /// @param ...params 
        void operator()(TParams... params) const
        {
            std::shared_lock lock(_listMutex);
            for (const auto& handler : _handlers)
                handler->Call(std::forward<TParams>(params)...);
        }

        /// @brief 
        /// @param handler 
        void operator+=(const EventHandler& handler) noexcept
        {
            if (!Contains(handler))
            {
                std::unique_lock lock(_listMutex);
                _handlers.push_back(handler);
            }
        }

        /// @brief 
        /// @param handler 
        void operator-=(const EventHandler& handler) noexcept
        {
            if (Contains(handler))
            {
                std::unique_lock lock(_listMutex);
                _handlers.erase(Find(handler));
            }
        }

        /// @brief 
        /// @param handler 
        /// @return 
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