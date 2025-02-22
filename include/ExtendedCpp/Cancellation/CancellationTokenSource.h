#ifndef Cancellation_CancellationTokenSource_H
#define Cancellation_CancellationTokenSource_H

#include <ExtendedCpp/Cancellation/CancellationToken.h>

/// @brief 
namespace ExtendedCpp::Cancellation
{
    /// @brief 
    class CancellationTokenSource final
    {
    private:
        std::atomic<bool*> _cancellationRequest;
        CancellationToken* _token;

    public:
        /// @brief 
        CancellationTokenSource() noexcept;

        /// @brief 
        ~CancellationTokenSource();

        /// @brief 
        /// @return 
        [[nodiscard]]
        const CancellationToken* Token() const noexcept;

        /// @brief 
        void Cancel() noexcept;
    };
}

#endif