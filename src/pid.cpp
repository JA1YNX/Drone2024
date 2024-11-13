#include "pid.h"

namespace PID_F
{
    double Pid::calc(double y, double r)
    {
        double ret = 0;
        double e = r - y;
        ret += e * KP;          // P項
        ret += (e - pree) * KD; // D項
        ie += (e + pree) / 2;
        ret += ie * KI; // I項
        return ret;
    }
    Pid::Pid(double pre)
    {
        pree = pre;
    }
    Pid::~Pid()
    {
    }
    void Pid::reset()
    {
        ie = 0;
    }
}
