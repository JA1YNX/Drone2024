#ifndef LOG_H
#define LOG_H
#include <Arduino.h>
#include "user.h"

enum class LogLevel : int
{
    _0NONE = 0,
    _1ERROR = 1,
    _2WARN = 2,
    _3INFO = 3,
    _4DEBUG = 4,
    _9ALL = 9
};
namespace Log
{
    LogLevel logset(LogLevel l);
    LogLevel logget();

    static LogLevel def = LogLevel::_3INFO;

    inline int setdef(LogLevel ll)
    {
        def = ll;
        return 0;
    }

    template <typename T>
    int log(const T &l, LogLevel ll = def)
    {
        return (logget() >= ll) ? Serial.print(l) : 0;
    }
    template <typename T>
    int logln(const T &l, LogLevel ll = def)
    {
        return (logget() >= ll) ? Serial.println(l) : 0;
    }
} // namespace Log

#endif
