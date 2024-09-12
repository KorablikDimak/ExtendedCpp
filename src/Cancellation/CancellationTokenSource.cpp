#include <ExtendedCpp/Cancellation/CancellationTokenSource.h>

ExtendedCpp::Cancellation::CancellationTokenSource::CancellationTokenSource() noexcept
{
    _cancellationRequest.store(new bool(false));
    _token = new CancellationToken(_cancellationRequest.load());
}

ExtendedCpp::Cancellation::CancellationTokenSource::~CancellationTokenSource()
{
    delete _cancellationRequest.load();
    delete _token;
}

ExtendedCpp::Cancellation::CancellationToken* ExtendedCpp::Cancellation::CancellationTokenSource::Token() const noexcept
{
    return _token;
}

void ExtendedCpp::Cancellation::CancellationTokenSource::Cancel() const noexcept
{
    *_cancellationRequest.load() = true;
}