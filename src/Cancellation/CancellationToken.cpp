#include <ExtendedCpp/Cancellation/CancellationToken.h>

ExtendedCpp::Cancellation::CancellationToken::CancellationToken(bool* cancellationRequest) noexcept
{
    _isCancellationRequested.store(cancellationRequest);
}

bool ExtendedCpp::Cancellation::CancellationToken::IsCancellationRequested() const noexcept
{
    return *_isCancellationRequested.load();
}