#include <ExtendedCpp/Cancellation/CancellationTokenSource.h>

ExtendedCpp::Cancellation::CancellationTokenSource::CancellationTokenSource() noexcept
{
    _cancellationRequest.store(false);
}

ExtendedCpp::Cancellation::CancellationToken ExtendedCpp::Cancellation::CancellationTokenSource::Token() const noexcept
{
    return CancellationToken(&_cancellationRequest);
}

void ExtendedCpp::Cancellation::CancellationTokenSource::Cancel() noexcept
{
    _cancellationRequest.store(true);
}