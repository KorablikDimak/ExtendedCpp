#include "Common/CancellationToken.h"

Common::CancellationToken::CancellationToken(bool* cancellationRequest) noexcept
{
    _isCancellationRequested.store(cancellationRequest);
}

bool Common::CancellationToken::IsCancellationRequested() const noexcept
{
    return *_isCancellationRequested.load();
}