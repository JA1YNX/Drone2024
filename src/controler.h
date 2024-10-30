#pragma once
#include "./conf.h"

// 制御用クラス
class contloler
{
public:
  // コンストラクタ
  contloler(user<int> set_);
  // プロポセットアップ
  void setup();
  // プロポ読み込み
  user<int> read();
  // 入力ピン出力
  user<int> pin() const;
  // data
  user<int> data() const;

private:
  // 入力ピン格納用変数構造体
  user<int> set;
  user<int> c;  // スタック用変数
  user<int> ud; // 標準値
};
