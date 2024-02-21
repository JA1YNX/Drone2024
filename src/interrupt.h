#include "conf.h"

class interrupt
{
public:
    interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
    user out();
private:
    void interrupt_fun();
    user read;
    user count;
    hw_timer_t * timer = NULL;
};

interrupt::interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{
    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer,&interrupt_fun,true);
    timerAlarmWrite(timer,interrupt_clock,true);
    timerAlarmEnable(timer);
}

user interrupt::out()
{
    return read;
}

void interrupt::interrupt_fun()
{

}
