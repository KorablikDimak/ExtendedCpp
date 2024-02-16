#ifndef Common_ErrorInfo_H
#define Common_ErrorInfo_H

#include <string>
#include <memory>

#include "ErrorCode.h"

namespace Common
{
    class ErrorInfo
    {
    private:
        ErrorCode _errorCode;
        std::string _description;

    public:
        typedef std::shared_ptr<ErrorInfo> Ptr;

        ErrorInfo(ErrorCode errorCode, const std::string& description);
        ~ErrorInfo() = default;

        [[nodiscard]] ErrorCode ErrorCode() const;
        std::string What();
    };
}

#endif