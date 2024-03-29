#ifndef Common_CancellationToken_H
#define Common_CancellationToken_H

#include <atomic>

namespace Common
{
    class CancellationToken final
    {
    private:
        std::atomic<bool*> _isCancellationRequested;

    public:
        explicit CancellationToken(bool* cancellationRequest);
        ~CancellationToken() = default;

        [[nodiscard]]
        bool IsCancellationRequested() const;
    };
}

#endif