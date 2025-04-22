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
    Pid::Pid(double p, double i, double d, double pre)
    {
        KP = p;
        KI = i;
        KD = d;
        pree = pre;
    }
    Pid::~Pid()
    {
    }
    void Pid::reset(double pre)
    {
        ie = 0;
        pree = pre;
    }
}
