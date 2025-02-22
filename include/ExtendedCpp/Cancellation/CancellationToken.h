#ifndef Cancellation_CancellationToken_H
#define Cancellation_CancellationToken_H

#include <atomic>

namespace ExtendedCpp::Cancellation
{
    /// @brief 
    class CancellationToken final
    {
    private:
        std::atomic<bool*> _isCancellationRequested;

    public:
        /// @brief 
        /// @param cancellationRequest 
        explicit CancellationToken(bool* cancellationRequest) noexcept;
        
        /// @brief 
        ~CancellationToken() = default;

        /// @brief 
        /// @return 
        [[nodiscard]]
        bool IsCancellationRequested() const noexcept;
    };
}

#endif