#ifndef Cancellation_CancellationTokenSource_H
#define Cancellation_CancellationTokenSource_H

#include <atomic>

#include <ExtendedCpp/Cancellation/CancellationToken.h>

/// @brief 
namespace ExtendedCpp::Cancellation
{
    /// @brief 
    class CancellationTokenSource final
    {
    private:
        std::atomic<bool> _cancellationRequest;

    public:
        /// @brief 
        CancellationTokenSource() noexcept
        {
            _cancellationRequest.store(false);
        }

        /// @brief 
        /// @return 
        [[nodiscard]]
        CancellationToken Token() const noexcept
        {
            return CancellationToken(&_cancellationRequest);
        }

        /// @brief 
        void Cancel() noexcept
        {
            _cancellationRequest.store(true);
        }
    };
}

#endif