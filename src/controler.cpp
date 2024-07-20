#include "./controler.h"

user<int> contloler::pin()
{
  return set;
}

contloler::contloler(user<int> set_)
{
  set = set_;
}
void contloler::setup()
{
  //モード設定
  pinMode(set.x, INPUT);
  pinMode(set.y, INPUT);
  pinMode(set.z, INPUT);
  pinMode(set.turn, INPUT);
  //値リセット
  c_x = 0;
  c_y = 0;
  c_z = 0;
  c_turn = 0;
  delay(5000);
  ud = user<double>{1500,1500,900,1500};
  return;
}

user<double> contloler::read()
{
  c_x = ((static_cast<int>(pulseIn(set.x,HIGH,100000))-ud.x)*(1.0)/10.0)*0.12*x_;
  c_y = ((static_cast<int>(pulseIn(set.y,HIGH,100000))-ud.y)*(-1.0)/10.0)*0.12*y_;
  c_z = ((static_cast<int>(pulseIn(set.z,HIGH,100000))-ud.z)*(1.0)/8.0)*0.15*x_;
  c_turn = ((static_cast<int>(pulseIn(set.turn,HIGH,100000))-ud.turn)*(-1.0)/10.0)*0.11*t_;

  bt.print("   cx:");
  bt.print(c_x);
  bt.print("   cy:");
  bt.print(c_y);
  bt.print("   cz:");
  bt.print(c_z);
  bt.print("   ct:");
  bt.print(c_turn);
  bt.print("     ");
  return user<double>{static_cast<double>(c_x), static_cast<double>(c_y), static_cast<double>(c_z), static_cast<double>(c_turn)};
}
