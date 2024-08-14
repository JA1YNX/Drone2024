#pragma once
#include "./conf.h"

//制御用クラス
class contloler {
public:
  //コンストラクタ
  contloler(user<int> set_);
  //入力ピン格納用変数構造体
  user<int> set;
  //プロポセットアップ
  void setup();
  //プロポ読み込み
  user<int> read();
  //入力ピン出力
  user<int> pin();
private:
  //スタック用変数
  int c_x;
  int c_y;
  int c_z;
  int c_turn;
  user<int> ud;//標準値
};

//今使ってるピン返すやつ
user<int> contloler::pin()
{
  return set;
}
//コンストラクタピン渡す
contloler::contloler(user<int> set_)
{
  set = set_;
}
//ピン設定
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
  ud = user<int>{1500,1500,1000,1500};
  return;
}
//読んで返す
user<int> contloler::read()
{
  c_x = ((static_cast<int>(pulseIn(set.x,HIGH,15000))-ud.x)*(1.0)/10.0)*0.12*x_;
  c_y = ((static_cast<int>(pulseIn(set.y,HIGH,15000))-ud.y)*(-1.0)/10.0)*0.12*y_;
  c_z = ((static_cast<int>(pulseIn(set.z,HIGH,15000))-ud.z)*(1.0)/8.0)*0.13*x_;
  c_turn = ((static_cast<int>(pulseIn(set.turn,HIGH,15000))-ud.turn)*(-1.0)/10.0)*0.11*t_;
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
  return user<int>{static_cast<int>(c_x), static_cast<int>(c_y), static_cast<int>(c_z), static_cast<int>(c_turn)};
}
