#ifndef Common_CancellationTokenSource_H
#define Common_CancellationTokenSource_H

#include "CancellationToken.h"

namespace Common
{
    class CancellationTokenSource final
    {
    private:
        std::atomic<bool*> _cancellationRequest;
        CancellationToken* _token;

    public:
        CancellationTokenSource() noexcept;
        ~CancellationTokenSource();

        [[nodiscard]]
        CancellationToken* Token() const noexcept;
        void Cancel() const noexcept;
    };
}

#endif