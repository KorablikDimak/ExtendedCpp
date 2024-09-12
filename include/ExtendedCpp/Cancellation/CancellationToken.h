#ifndef Common_CancellationToken_H
#define Common_CancellationToken_H

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