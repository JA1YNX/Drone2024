#include "log.h"

namespace Log
{
    LogLevel lstatus = LogLevel::_3INFO;
    LogLevel init(unsigned long speed)
    {
        Serial.begin(speed);
        logln("Log Init", LogLevel::_3INFO);
        return lstatus;
    }
    LogLevel logset(LogLevel l)
    {
        return lstatus = l;
    }
    LogLevel logget()
    {
        return lstatus;
    }
} // namespace Log