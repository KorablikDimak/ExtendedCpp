#ifndef Events_EventHandler_H
#define Events_EventHandler_H

/// @brief 
namespace ExtendedCpp::Events
{
    /// @brief 
    /// @tparam TParams
    template<typename ...TParams>
    class IEventHandler
    {
    public:
        /// @brief 
        IEventHandler() noexcept = default;

        /// @brief 
        virtual ~IEventHandler() = default;

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const IEventHandler& other) const noexcept
        {
            return IsEquals(other);
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const IEventHandler& other) const noexcept
        {
            return !IsEquals(other);
        }

        /// @brief 
        /// @param params
        virtual void Call(TParams&&... params) const = 0;

    private:
        /// @brief 
        /// @param other 
        /// @return 
        bool IsEquals(const IEventHandler& other) const noexcept
        {
            return std::addressof(*this) == std::addressof(other);
        }
    };
}

#endif