#include "Common/ErrorInfo.h"

Common::ErrorInfo::ErrorInfo(const Common::ErrorCode errorCode, const std::string& description)
{
    _errorCode = errorCode;
    _description = description;
}

Common::ErrorCode Common::ErrorInfo::ErrorCode() const
{
    return _errorCode;
}

std::string Common::ErrorInfo::What()
{
    return _description;
}