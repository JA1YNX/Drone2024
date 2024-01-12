#include <Arduino.h>
#include "BNO055.hpp"
#include "motor.hpp"
#include "user.hpp"



motor m(32,33,25,26,1,2,3,4);
user u;

void setup2(void)
{
  bno_setup();

  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  turn = 0;
}


void loop2(void)
{
  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  m.rotate();
  m.d = 60+(z-euler.z())*50;
  m.c1 = ((x-euler.x())+(y-euler.y())*(-1))*500+turn/2;
  m.c2 = ((x-euler.x())*(-1)+(y-euler.y())*(-1))*500-turn/2;
  m.c3 = ((x-euler.x())+(y-euler.y()))*500+turn/2;
  m.c4 = ((x-euler.x())*(-1)+(y-euler.y()))*500-turn/2;

  m.c1 += (u.x+u.y*(-1))+u.turn;
  m.c2 += (u.x*(-1)+u.y*(-1))-u.turn;
  m.c3 += (u.x+u.y)+u.turn;
  m.c4 += (u.x*(-1)+u.y)-u.turn;

}