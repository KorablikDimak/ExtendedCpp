#ifndef InfoLog_LogLevel_H
#define InfoLog_LogLevel_H

namespace ExtendedCpp::InfoLog
{
    /// @brief 
    enum class LogLevel
    {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    };
}

#endif