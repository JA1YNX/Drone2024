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
  user<double> read();
  //入力ピン出力
  user<int> pin();
private:
  //スタック用変数
  int c_x;
  int c_y;
  int c_z;
  int c_turn;
  user<double> ud;//標準値
};