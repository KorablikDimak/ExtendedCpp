module;

#include <atomic>

export module ExtendedCpp.Common.CancellationTokenSource;

import ExtendedCpp.Common.CancellationToken;

export namespace ExtendedCpp::Common
{
    class CancellationTokenSource final
    {
    private:
        std::atomic<bool*> _cancellationRequest{};
        CancellationToken* _token{};

    public:
        CancellationTokenSource() noexcept = default;
        ~CancellationTokenSource() = default;

        [[nodiscard]]
        CancellationToken* Token() const noexcept
        {
            return _token;
        }

        void Cancel() const noexcept
        {
            *_cancellationRequest.load() = true;
        }
    };
}