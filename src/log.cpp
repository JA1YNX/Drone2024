#include "log.h"

namespace Log
{
    LogLevel lstatus = LogLevel::_3INFO;
    LogLevel logset(LogLevel l)
    {
        return lstatus = l;
    }
    LogLevel logget()
    {
        return lstatus;
    }
} // namespace Log