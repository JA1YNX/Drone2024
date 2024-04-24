#pragma once
#include "./conf.h"

//制御用クラス
class contloler {
public:
  //コンストラクタ
  contloler(user set_);
  //入力ピン格納用変数構造体
  user set;
  //プロポセットアップ
  void setup();
  //プロポ読み込み
  user read();
  //入力ピン出力
  user pin();
private:
  //スタック用変数
  int c_x;
  int c_y;
  int c_z;
  int c_turn;
  user ud;//標準値
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
#ifndef interrupt_on
  ud = user{analogRead(set.x),analogRead(set.y),analogRead(set.z),analogRead(set.turn)};
#endif
#ifdef interrupt_on
  ud = user{1500,1500,900,1500};
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
  c_x = ((static_cast<int>(pulseIn(set.x,HIGH,100000))-ud.x)*(1.0)/10.0)*0.12;
  c_y = ((static_cast<int>(pulseIn(set.y,HIGH,100000))-ud.y)*(-1.0)/10.0)*0.12;
  c_z = ((static_cast<int>(pulseIn(set.z,HIGH,100000))-ud.z)*(1.0)/8.0)*0.15;
  c_turn = ((static_cast<int>(pulseIn(set.turn,HIGH,100000))-ud.turn)*(-1.0)/10.0)*0.11;

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
