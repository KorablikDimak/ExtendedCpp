#include "Common/CancellationTokenSource.h"

Common::CancellationTokenSource::CancellationTokenSource() noexcept
{
    _cancellationRequest.store(new bool(false));
    _token = new CancellationToken(_cancellationRequest.load());
}

Common::CancellationTokenSource::~CancellationTokenSource()
{
    delete _cancellationRequest.load();
    delete _token;
}

Common::CancellationToken* Common::CancellationTokenSource::Token() const noexcept
{
    return _token;
}

void Common::CancellationTokenSource::Cancel() const noexcept
{
    *_cancellationRequest.load() = true;
}