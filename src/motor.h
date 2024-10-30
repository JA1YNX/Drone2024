#pragma once
#include "./conf.h"

class motor
{ // モーターチャンネルとピン設定
public:
  int nf;              // モーターの回転ON/OFF
  int def;             // もとになる数値
  int c1;              // 左前変更値
  int c2;              // 右前変更値
  int c3;              // 左後変更値
  int c4;              // 右後変更値
  void rotate() const; // 設定反映
  // motor(int pin_1, int pin_2, int pin_3, int pin_4): pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4){}
  motor(user<int> pin_) : pin(pin_) {}
  void setup() const; // 初期設定
  void stop();        // 強制停止
private:
  user<int> pin; // モーター制御用ピン
};
