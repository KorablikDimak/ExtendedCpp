#ifndef Common_ErrorCode_H
#define Common_ErrorCode_H

namespace Common
{
    enum class ErrorCode
    {
        NoError = 0,
        Undefined = 1,
        RuntimeError = 2,

        NullPointer = 10,

        IncorrectArgument = 20,
        OutOfRange = 21,

        ParseFailed = 30,
        JsonParseFailed = 31,
        XmlParseFailed = 32,
        TextParseFailed = 33
    };
}

#endif