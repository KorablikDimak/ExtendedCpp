#include <ExtendedCpp/Cancellation/CancellationTokenSource.h>
#include <ExtendedCpp/Cancellation/CancellationToken.h>

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

const ExtendedCpp::Cancellation::CancellationToken* ExtendedCpp::Cancellation::CancellationTokenSource::Token() const noexcept
{
    return _token;
}

void ExtendedCpp::Cancellation::CancellationTokenSource::Cancel() noexcept
{
    *_cancellationRequest.load() = true;
}