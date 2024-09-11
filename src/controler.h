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
  user<int> c;
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
  //delay(5000);
  ud = user<int>{1520,1520,1110,1520};
  return;
}
//読んで返す
user<int> contloler::read()
{
  c.x = (static_cast<int>(pulseIn(set.x,HIGH,20000))-ud.x)/8;
  c.y = (static_cast<int>(pulseIn(set.y,HIGH,20000))-ud.y)/8;
  c.z = (static_cast<int>(pulseIn(set.z,HIGH,20000))-ud.z)/15;
  c.turn = (static_cast<int>(pulseIn(set.turn,HIGH,20000))-ud.turn)/8;
#ifdef output
  bt.print("   cx:");
  bt.print(c.x);
  bt.print("   cy:");
  bt.print(c.y);
  bt.print("   cz:");
  bt.print(c.z);
  bt.print("   ct:");
  bt.print(c.turn);
  bt.print("     ");
#endif
  return {c.x,c.y,c.z,c.turn};
}
