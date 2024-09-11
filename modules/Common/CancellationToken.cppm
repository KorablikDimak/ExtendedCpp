module;

#include <atomic>

export module ExtendedCpp.Common.CancellationToken;

export namespace ExtendedCpp::Common
{
    class CancellationToken final
    {
    private:
        std::atomic<bool*> _isCancellationRequested;

    public:
        explicit CancellationToken(bool* cancellationRequest) noexcept
        {
            _isCancellationRequested.store(cancellationRequest);
        }

        ~CancellationToken() = default;

        [[nodiscard]]
        bool IsCancellationRequested() const noexcept
        {
            return *_isCancellationRequested.load();
        }
    };
}