#include "Common/CancellationToken.h"

Common::CancellationToken::CancellationToken(bool* cancellationRequest)
{
    _isCancellationRequested.store(cancellationRequest);
}

bool Common::CancellationToken::IsCancellationRequested() const
{
    return *_isCancellationRequested.load();
}