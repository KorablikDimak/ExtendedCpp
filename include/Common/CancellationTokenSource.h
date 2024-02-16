#ifndef Common_CancellationTokenSource_H
#define Common_CancellationTokenSource_H

#include "CancellationToken.h"

namespace Common
{
    class CancellationTokenSource
    {
    private:
        std::atomic<bool*> _cancellationRequest;
        CancellationToken* _token;

    public:
        CancellationTokenSource();
        ~CancellationTokenSource();

        [[nodiscard]]
        CancellationToken* Token() const;
        void Cancel() const;
    };
}

#endif