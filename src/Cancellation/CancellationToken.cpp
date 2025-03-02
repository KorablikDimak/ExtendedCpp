#include <ExtendedCpp/Cancellation/CancellationToken.h>

ExtendedCpp::Cancellation::CancellationToken::CancellationToken(const std::atomic<bool>* cancellationRequest) noexcept
{
    _cancellationRequest = cancellationRequest;
}

bool ExtendedCpp::Cancellation::CancellationToken::IsCancellationRequested() const noexcept
{
    return _cancellationRequest->load();
}