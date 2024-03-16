#include "./conf.h"

uesr out();
void inter_x();
void inter_y();
void inter_z();
void inter_turn();
void countup();

hw_timer_t * timer = NULL;
   
void setup()
{
    contloler c(32, 33, 34, 35, 12, 13, user{33, 35, 32, 34});
    static user inter_count;
    static user inter_out;
    attachInterrupt(inter_x,33,RISING);
    attachInterrupt(inter_y,35,RISING);
    attachInterrupt(inter_z,32,RISING);
    attachInterrupt(inter_turn,34,RISING);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &countup, true);
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
}

uesr out()
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
