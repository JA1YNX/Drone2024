#pragma once
#include "./conf.h"

class interrupt
{
public:
    interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
    user out();
private:
    static void interrupt_fun();
    hw_timer_t * timer = NULL;
    static user set;
    static user read;
    volatile static user count;
    TaskHandle_t thp[1];//マルチスレッドのタスクハンドル格納用
    static void xcore();
};

interrupt::interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{

    /*
    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer,&interrupt_fun,true);
    timerAlarmWrite(timer,interrupt_clock,true);
    timerAlarmEnable(timer);
    */
    set = set_;
    xTaskCreatePinnedToCore(xcore, "Core0a", 4096, NULL, 3, &thp[0], 0); 
}

user interrupt::out()
{
    return read;
}

void interrupt::xcore()
{
    while(1)
    {
        interrupt_fun();
    }
}

void interrupt::interrupt_fun()
{
    bool x = digitalRead(set.x);
    bool y = digitalRead(set.y);
    bool z = digitalRead(set.z);
    bool turn = digitalRead(set.turn);
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
    if(turn)
    {
        count.turn ++;
    }
    else if(!count.turn == 0)
    {
        read.turn  = count.turn;
        count.turn = 0;
    }
}
