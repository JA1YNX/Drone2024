#include <Arduino.h>

class contloler {
  public:
    int pin_in1;
    int pin_in2;
    int pin_in3;
    int pin_in4;
    int pin_in5;
    int pin_in6;
    user set;
    void setup();
    user read();
    contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
  private:
    int c_x;
    int c_y;
    int c_z;
    int c_turn;    
};

contloler::contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{
  pin_in1 = pin1;
  pin_in2 = pin2;
  pin_in3 = pin3;
  pin_in4 = pin4;
  pin_in5 = pin5;
  pin_in6 = pin6;
  set = set_;
}
void contloler::setup()
{
  pinMode(pin_in1, INPUT_PULLUP);
  pinMode(pin_in2, INPUT_PULLUP);
  pinMode(pin_in3, INPUT_PULLUP);
  pinMode(pin_in4, INPUT_PULLUP);
  pinMode(pin_in5, INPUT_PULLUP);
  pinMode(pin_in6, INPUT_PULLUP);
  c_x = 0;
  c_y = 0;
  c_z = 0;
  c_turn = 0;
}

user contloler::read()
{
  c_x = (mode2 == 0?(c_x/2+(((mode == 0?analogRead(set.x):mode_set.x)-ud.x) * read_)*5):((mode == 0?analogRead(set.x):mode_set.x)-ud.x)*read_*10);
  c_y = (mode2 == 0?(c_y/2+(((mode == 0?analogRead(set.y):mode_set.y)-ud.y) * read_)*5):((mode == 0?analogRead(set.y):mode_set.y)-ud.y)*read_*10);
  c_z = (mode2 == 0?(c_z/2+(((mode == 0?analogRead(set.z):mode_set.z)-ud.z) * read_)*5):((mode == 0?analogRead(set.z):mode_set.z)-ud.z)*read_*10);
  c_turn = (mode2 == 0?(c_turn/2+(((mode == 0?analogRead(set.turn):mode_set.turn)-ud.turn) * read_)*5):((mode == 0?analogRead(set.turn):mode_set.turn)-ud.turn)*read_*10);
  /*
  int x = (analogRead(set.x) - ud.x) * read_*10;
  int y = (analogRead(set.y) - ud.y) * read_*10;
  int z = (analogRead(set.z) - ud.z) * read_*10;
  int turn = (analogRead(set.turn) - ud.turn) * read_*10;
  */
  bt.print("   cx:");
  bt.print(c_x);
  bt.print("   cy:");
  bt.print(c_y);
  bt.print("   cz:");
  bt.print(c_z);
  bt.print("   ct:");
  bt.print(c_turn);
  bt.print("     ");
  return user{c_x, c_y, c_z, c_turn};
}
