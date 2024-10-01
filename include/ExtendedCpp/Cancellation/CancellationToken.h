#ifndef Cancellation_CancellationToken_H
#define Cancellation_CancellationToken_H

#include <atomic>

namespace ExtendedCpp::Cancellation
{
    class CancellationToken final
    {
    private:
        std::atomic<bool*> _isCancellationRequested;

    public:
        explicit CancellationToken(bool* cancellationRequest) noexcept;
        ~CancellationToken() = default;

        [[nodiscard]]
        bool IsCancellationRequested() const noexcept;
    };
}

#endif