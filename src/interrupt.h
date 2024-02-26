#pragma once
#include <Arduino.h>
#include "conf.h"

class interrupt
{
public:
    interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
    user out();
private:
    user set;
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
    set = set_;
}

user interrupt::out()
{
    return read;
}

void interrupt::interrupt_fun()
{
    bool x = digitalread(set.x);
    bool y = digitalread(set.y);
    bool z = digitalread(set.z);
    bool turn = digitalread(set.turn);
    if(x)
    {
        count.x ++;
    }
    else if(!count.x == 0)
    {
        read.x  = count.x;
        count.x = 0;
    }
    if(y)
    {
        count.y ++;
    }
    else if(!count.y == 0)
    {
        read.y  = count.y;
        count.y = 0;
    }
    if(z)
    {
        count.z ++;
    }
    else if(!count.z == 0)
    {
        read.z  = count.z;
        count.z = 0;
    }
    if()
    {
        count. ++;
    }
    else if(!count.turn == 0)
    {
        read.turn  = count.turn;
        count.turn = 0;
    }
}
