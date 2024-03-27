#pragma once
#include "./conf.h"

class contloler {
public:
  contloler(user set_);
  user set;
  void setup();
  user read();
  user pin();
private:
  int c_x;
  int c_y;
  int c_z;
  int c_turn;
  user ud;//標準
};

user contloler::pin()
{
  return set;
}

contloler::contloler(user set_)
{
  set = set_;
}
void contloler::setup()
{
  pinMode(set.x, INPUT);
  pinMode(set.y, INPUT);
  pinMode(set.z, INPUT);
  pinMode(set.turn, INPUT);
  c_x = 0;
  c_y = 0;
  c_z = 0;
  c_turn = 0;
  delay(5000);
#ifndef interrupt_on
  ud = user{analogRead(set.x),analogRead(set.y),analogRead(set.z),analogRead(set.turn)};
#endif
#ifdef interrupt_on
  ud = user{1500,1500,1100,1500};
#endif
  return;
}

user contloler::read()
{
#ifndef interrupt_on
  c_x = (analogRead(set.x)-ud.x) * read_*5;
  c_y = (analogRead(set.y)-ud.y) * read_*5;
  c_z = (analogRead(set.z)-ud.z) * read_*5;
  c_turn = (analogRead(set.turn)-ud.turn) * read_*5;
#endif
#ifdef interrupt_on
  c_x = (static_cast<int>(pulseIn(set.x,HIGH,100000))-ud.x)*(-1)/100;
  c_y = (static_cast<int>(pulseIn(set.y,HIGH,100000))-ud.y)*(-1)/100;
  c_z = (static_cast<int>(pulseIn(set.z,HIGH,100000))-ud.z)/100;
  c_turn = (static_cast<int>(pulseIn(set.turn,HIGH,100000))-ud.turn)/100;

  //c_x = ((pulseIn(set.x,HIGH,100000)*-1)/10-ud.x)/10;
  //c_y = ((pulseIn(set.y,HIGH,100000)*-1)/10-ud.y)/10;
  //c_z = (pulseIn(set.z,HIGH,100000)/10-ud.z)/10;
  //c_turn = (pulseIn(set.turn,HIGH,100000)/10-ud.turn)/10;
#endif
#ifdef output
  bt.print("   cx:");
  bt.print(c_x);
  bt.print("   cy:");
  bt.print(c_y);
  bt.print("   cz:");
  bt.print(c_z);
  bt.print("   ct:");
  bt.print(c_turn);
  bt.print("     ");
#endif
  return user{c_x, c_y, c_z, c_turn};
}
