#include <Arduino.h>
#include "user.h"

namespace Log
{
    enum class LogLevel : int
    {
        _0NONE = 0,
        _1ERROR = 1,
        _2WARN = 2,
        _3INFO = 3,
        _4DEBUG = 4,
        _9ALL = 9
    };
    LogLevel logset(LogLevel l);
    LogLevel logget();

    template <typename T>
    int log(const T &l, LogLevel ll = LogLevel::_3INFO)
    {
        return (logget() >= ll) ? Serial.print(l) : 0;
    }
    template <typename T>
    int logln(const T &l, LogLevel ll = LogLevel::_3INFO)
    {
        return (logget() >= ll) ? Serial.println(l) : 0;
    }
} // namespace Log
