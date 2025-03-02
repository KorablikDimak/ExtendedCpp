#ifndef Cancellation_CancellationToken_H
#define Cancellation_CancellationToken_H

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
        explicit CancellationToken(const std::atomic<bool>* cancellationRequest) noexcept;
        
        /// @brief 
        ~CancellationToken() = default;

        /// @brief 
        /// @return 
        [[nodiscard]]
        bool IsCancellationRequested() const noexcept;
    };
}

#endif