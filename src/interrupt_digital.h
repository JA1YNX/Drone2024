#pragma once
#include "./conf.h"

user out();
void inter_x();
void inter_y();
void inter_z();
void inter_turn();
void countup();
user inter_count;
user inter_out;

hw_timer_t * timer = NULL;
   
void inter_setup()
{
    contloler c(32, 33, 34, 35, 12, 13, user{33, 35, 32, 34});
    attachInterrupt(33,inter_x,RISING);
    attachInterrupt(35,inter_y,RISING);
    attachInterrupt(32,inter_z,RISING);
    attachInterrupt(34,inter_turn,RISING);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &countup, true);
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
}

user out()
{
    return inter_out;
}

void countup()
{
    inter_count.x++;
    inter_count.y++;
    inter_count.z++;
    inter_count.turn++;
}

void inter_x()
{
    inter_out.x = inter_count.x;
}
void inter_y()
{
    inter_out.y = inter_count.y;
}
void inter_z()
{
    inter_out.z = inter_count.z;
}
void inter_turn()
{
    inter_out.turn = inter_count.turn;
}
