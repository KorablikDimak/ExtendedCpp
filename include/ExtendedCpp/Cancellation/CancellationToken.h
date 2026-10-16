#ifndef CANCELLATION_CANCELLATION_TOKEN_H
#define CANCELLATION_CANCELLATION_TOKEN_H

#include <atomic>

/// @brief
namespace ExtendedCpp::Cancellation
{
    /// @brief
    class CancellationToken final
    {
    private:
        const std::atomic<bool>* _cancellationRequest;

    public:
        /// @brief 
        /// @param cancellationRequest
        explicit CancellationToken(const std::atomic<bool>* cancellationRequest) noexcept :
            _cancellationRequest(cancellationRequest) {}

        /// @brief 
        /// @return
        [[nodiscard]]
        bool IsCancellationRequested() const noexcept
        {
            return _cancellationRequest->load();
        }
    };
}

#endif