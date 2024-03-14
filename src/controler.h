#pragma once
#include "./conf.h"
#include "./interrupt.h"

class contloler {
public:
  contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
  int pin_in1;
  int pin_in2;
  int pin_in3;
  int pin_in4;
  int pin_in5;
  int pin_in6;
  user set;
  void setup();
  user read();
private:
  int c_x;
  int c_y;
  int c_z;
  int c_turn;
  user ud;//標準
};
#ifdef interrupt_on
  interrupt i(32, 33, 34, 35, 12, 13, user{33, 35, 32, 34});
#endif

contloler::contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{
  pin_in1 = pin1;
  pin_in2 = pin2;
  pin_in3 = pin3;
  pin_in4 = pin4;
  pin_in5 = pin5;
  pin_in6 = pin6;
  set = set_;
#ifdef interrupt_on
  i = interrupt{pin_in1,pin_in2,pin_in3,pin_in4,pin_in5,pin_in6,set};
#endif
}
void contloler::setup()
{
  pinMode(pin_in1, INPUT);
  pinMode(pin_in2, INPUT);
  pinMode(pin_in3, INPUT);
  pinMode(pin_in4, INPUT);
  pinMode(pin_in5, INPUT);
  pinMode(pin_in6, INPUT);
  c_x = 0;
  c_y = 0;
  c_z = 0;
  c_turn = 0;
  delay(5000);
  ud = user{analogRead(33),analogRead(35),analogRead(32),analogRead(34)};
#ifdef interrupt_on
  ud = user{i.out().x,i.out().y,i.out().z,i.out().turn};
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
  c_x = (i.out().x-ud.x) *5;
  c_y = (i.out().y-ud.y) *5;
  c_z = (i.out().z-ud.z) *5;
  c_turn = (i.out().turn-ud.turn) * read_*5;
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
