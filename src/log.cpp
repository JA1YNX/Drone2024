#include "log.h"

namespace Log
{
    int lstatus = 1;
    int logset(int l)
    {
        return lstatus = l;
    }
    int log(const String &l)
    {
        return (lstatus == 1) ? Serial.print(l) : 0;
    }
    int logln(const String &l)
    {
        return (lstatus == 1) ? Serial.println(l) : 0;
    }
} // namespace Log