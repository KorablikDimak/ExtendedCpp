#ifndef Common_CancellationTokenSource_H
#define Common_CancellationTokenSource_H

#include <ExtendedCpp/Cancellation/CancellationToken.h>

namespace ExtendedCpp::Cancellation
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
        const CancellationToken* Token() const noexcept;
        void Cancel() noexcept;
    };
}

#endif