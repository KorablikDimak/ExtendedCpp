#ifndef Common_Error_H
#define Common_Error_H

#include <stack>

#include "ErrorInfo.h"

namespace Common
{
    class ErrorTrace
    {
    private:
        std::stack<ErrorInfo> _stackTrace;

    public:
        typedef std::shared_ptr<ErrorTrace> Ptr;

        ErrorTrace() = default;
        ~ErrorTrace() = default;

        void Push(const ErrorInfo& errorInfo);
        ErrorInfo Pop();
        [[nodiscard]]
        std::string ToString() const;
    };
}

#endif