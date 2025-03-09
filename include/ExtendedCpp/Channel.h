#ifndef Common_Channel_H
#define Common_Channel_H

#include <optional>
#include <tuple>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

/// @brief 
namespace ExtendedCpp
{
    /// @brief 
    enum class ChannelType
    {
        Sender,
        Receiver,
        None
    };

    /// @brief 
    /// @tparam TMessageType 
    /// @tparam TChannelType 
    template<typename TMessageType, ChannelType TChannelType = ChannelType::None>
    class Channel final
    {
    public:
        /// @brief 
        using MessageType = TMessageType;

        /// @brief 
        struct ControlBlock final
        {
            std::queue<MessageType> messageBus;
            bool hasMessage = false;
            mutable std::mutex mutex;
            std::condition_variable messageAvaible;

            unsigned sendersCount = 0;
            unsigned receiversCount = 0;
            bool closed = false;
        };

        /// @brief 
        using ControlBlockType = Channel<MessageType, ChannelType::None>::ControlBlock;

        /// @brief 
        static constexpr bool IsSender = (TChannelType == ChannelType::Sender);

        /// @brief 
        static constexpr bool IsReceiver = (TChannelType == ChannelType::Receiver);

        /// @brief 
        /// @return 
        static std::pair<Channel<MessageType, ChannelType::Sender>, Channel<MessageType, ChannelType::Receiver>>
        Create() noexcept
        {
            auto controlBlock = std::make_shared<ControlBlockType>();
            return std::make_pair(Channel<MessageType, ChannelType::Sender>(controlBlock),
                                  Channel<MessageType, ChannelType::Receiver>(controlBlock));
        }

        /// @brief 
        /// @param other 
        Channel(const Channel& other) noexcept
        {
            std::lock_guard lock(other._controlBlock->mutex);
            _controlBlock = other._controlBlock;

            if (IsSender)
                ++_controlBlock->sendersCount;
            else if (IsReceiver)
                ++_controlBlock->receiversCount;
        }

        /// @brief 
        /// @param other 
        Channel(Channel&& other) noexcept
        {
            std::lock_guard lock(other._controlBlock->mutex);
            _controlBlock = other._controlBlock;
            other._controlBlock = nullptr;
        }

        /// @brief 
        /// @param other 
        /// @return 
        Channel& operator=(const Channel& other) noexcept
        {
            std::lock_guard lock(other._controlBlock->mutex);
            _controlBlock = other._controlBlock;

            if (IsSender)
                ++_controlBlock->sendersCount;
            else if (IsReceiver)
                ++_controlBlock->receiversCount;

            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        Channel& operator=(Channel&& other) noexcept
        {
            std::lock_guard lock(other._controlBlock->mutex);
            _controlBlock = other._controlBlock;
            other._controlBlock = nullptr;
            return *this;
        }

        /// @brief 
        ~Channel()
        {
            if (_controlBlock == nullptr)
                return;

            {
                std::lock_guard lock(_controlBlock->mutex);

                if (IsSender)
                    --_controlBlock->sendersCount;
                else if (IsReceiver)
                    --_controlBlock->receiversCount;
        
                if (!_controlBlock->sendersCount || _controlBlock->receiversCount)
                    _controlBlock->closed = true;
            }

            _controlBlock->messageAvaible.notify_one();
        }

        /// @brief 
        /// @param message 
        void Send(const MessageType& message) noexcept requires IsSender
        {
            {
                std::lock_guard lock(_controlBlock->mutex);
                if (_controlBlock->closed)
                    return;
                _controlBlock->messageBus.push(message);
                _controlBlock->hasMessage = true;
            }
            _controlBlock->messageAvaible.notify_one();
        }

        /// @brief 
        /// @param message 
        void Send(MessageType&& message) noexcept requires IsSender
        {
            {
                std::lock_guard lock(_controlBlock->mutex);
                if (_controlBlock->closed)
                    return;
                _controlBlock->messageBus.push(std::move(message));
                _controlBlock->hasMessage = true;
            }
            _controlBlock->messageAvaible.notify_one();
        }

        /// @brief 
        /// @return 
        MessageType Recive() requires IsReceiver
        {
            std::unique_lock lock(_controlBlock->mutex);
            _controlBlock->messageAvaible.wait(lock, [this]()
            { 
                if (_controlBlock->closed && !_controlBlock->hasMessage)
                    throw std::domain_error("Channel was closed.");
                return _controlBlock->hasMessage; 
            });

            MessageType message = _controlBlock->messageBus.front();
            _controlBlock->messageBus.pop();
            if (_controlBlock->messageBus.empty())
                _controlBlock->hasMessage = false;
            return message;
        }

        /// @brief 
        /// @return 
        std::optional<MessageType> TryRecive() noexcept requires IsReceiver
        {
            std::lock_guard lock(_controlBlock->mutex);
            if (!_controlBlock->hasMessage || _controlBlock->closed) 
                return std::nullopt;
                
            MessageType message = _controlBlock->messageBus.front();
            _controlBlock->messageBus.pop();
            if (_controlBlock->messageBus.empty())
                _controlBlock->hasMessage = false;
            return message;
        }

        /// @brief 
        /// @return 
        [[nodiscard]]
        bool Closed() const noexcept
        {
            std::lock_guard lock(_controlBlock->mutex);
            return _controlBlock->closed;
        }

        /// @brief 
        [[nodiscard]]
        operator bool() const noexcept requires IsSender
        {
            return Closed();
        }

        /// @brief 
        [[nodiscard]]
        operator bool() const noexcept requires IsReceiver
        {
            std::lock_guard lock(_controlBlock->mutex);
            return !_controlBlock->closed || _controlBlock->hasMessage;
        }

        /// @brief 
        /// @param controlBlock 
        Channel(std::shared_ptr<ControlBlockType> controlBlock) noexcept requires IsSender || IsReceiver
        {
            _controlBlock = controlBlock;

            if (IsSender)
                ++_controlBlock->sendersCount;
            else if (IsReceiver)
                ++_controlBlock->receiversCount;
        }

    private:
        std::shared_ptr<ControlBlockType> _controlBlock;
    };

    /// @brief 
    /// @tparam TMessageType 
    /// @param sender 
    /// @param message 
    /// @return 
    template<typename TMessageType>
    Channel<std::remove_reference_t<TMessageType>, ChannelType::Sender>& operator<<
    (Channel<std::remove_reference_t<TMessageType>, ChannelType::Sender>& sender, TMessageType&& message) noexcept
    {
        sender.Send(std::forward<TMessageType>(message));
        return sender;
    }

    /// @brief 
    /// @tparam TMessageType 
    /// @param message 
    /// @param sender 
    /// @return 
    template<typename TMessageType>
    Channel<std::remove_reference_t<TMessageType>, ChannelType::Sender>& operator>>
    (TMessageType&& message, Channel<std::remove_reference_t<TMessageType>, ChannelType::Sender>& sender) noexcept
    {
        sender.Send(std::forward<TMessageType>(message));
        return sender;
    }

    /// @brief 
    /// @tparam TMessageType 
    /// @param receiver 
    /// @param message 
    /// @return 
    template<typename TMessageType>
    Channel<TMessageType, ChannelType::Receiver>& operator>>
    (Channel<TMessageType, ChannelType::Receiver>& receiver, TMessageType& message)
    {
        message = receiver.Recive();
        return receiver;
    }

    /// @brief 
    /// @tparam TMessageType 
    /// @param message 
    /// @param receiver 
    /// @return 
    template<typename TMessageType>
    Channel<TMessageType, ChannelType::Receiver>& operator<<
    (TMessageType& message, Channel<TMessageType, ChannelType::Receiver>& receiver)
    {
        message = receiver.Recive();
        return receiver;
    }
}

#endif