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
    static void xcore(void *pvParameters);
};
user sum;
user sum_count;
user set;
user read_intr;
volatile user count;

interrupt::interrupt(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{

    set = set_;
    /*
    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer,&interrupt_fun,true);
    timerAlarmWrite(timer,interrupt_clock,true);
    timerAlarmEnable(timer);
    */
    xTaskCreatePinnedToCore(xcore, "xcore", 4096, NULL, 0, NULL, 0); 
}

user interrupt::out()
{
    return read_intr;
}

void interrupt::xcore(void *pvParameters)
{
    bool x,y,z,turn;
    while(1)
    {
        x = (analogRead(set.x)<3500);
        y = (analogRead(set.y)<3500);
        z = (analogRead(set.z)<3500);
        turn = (analogRead(set.turn)<3500);
        //Serial.print(x);
        //Serial.print(y);
        //Serial.print(z);
        //Serial.println(turn);
        if(x)
        {
            count.x++;
        }
        else if(!count.x == 0)
        {
            sum_count.x++;
            sum.x+=count.x;
            read_intr.x = sum.x/sum_count.x;

            //read_intr.x  = count.x;
            count.x = 0;
        }
        if(y)
        {
            count.y++;
        }
        else if(!count.y == 0)
        {
            sum_count.y++;
            sum.y+=count.y;
            read_intr.y  = sum.y/sum_count.y;

            //read_intr.y  = count.y;
            count.y = 0;
        }
        if(z)
        {
            count.z++;
        }
        else if(!count.z == 0)
        {
            sum_count.z++;
            sum.z+=count.z;
            read_intr.z  = sum.z/sum_count.z;

            //read_intr.z  = count.z;
            count.z = 0;
        }
        if(turn)
        {
            count.turn++;
        }
        else if(!count.turn == 0)
        {
            sum_count.turn++;
            sum.turn+=count.turn;
            read_intr.turn  = sum.turn/sum_count.turn;

            //read_intr.turn  = count.turn;
            count.turn = 0;
        }
    }
}

void interrupt::interrupt_fun()
{
    bool x,y,z,turn;
    x = (analogRead(set.x)<3500);
    y = (analogRead(set.y)<3500);
    z = (analogRead(set.z)<3500);
    turn = (analogRead(set.turn)<3500);
    //Serial.print(x);
    //Serial.print(y);
    //Serial.print(z);
    //Serial.println(turn);
    if(x)
    {
        count.x++;
    }
    else if(!count.x == 0)
    {
        read_intr.x  = count.x;
        count.x = 0;
    }
    if(y)
    {
        count.y++;
    }
    else if(!count.y == 0)
    {
        read_intr.y  = count.y;
        count.y = 0;
    }
    if(z)
    {
        count.z++;
    }
    else if(!count.z == 0)
    {
        read_intr.z  = count.z;
        count.z = 0;
    }
    if(turn)
    {
        count.turn++;
    }
    else if(!count.turn == 0)
    {
        read_intr.turn  = count.turn;
        count.turn = 0;
    }
}
