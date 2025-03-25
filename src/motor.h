#pragma once
#include "./conf.h"

#ifdef __cplusplus
extern "C"
{
#endif

  class motor
  { // モーターチャンネルとピン設定
  public:
    /// @brief モーターの回転ON/OFF
    int nf;
    /// @brief もとになる数値
    int def;
    /// @brief 左前変更値
    int c1;
    /// @brief 右前変更値
    int c2;
    /// @brief 左後変更値
    int c3;
    /// @brief 右後変更値
    int c4;
    /// @brief 設定反映
    void rotate() const;
    // motor(int pin_1, int pin_2, int pin_3, int pin_4): pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4){}

    /// @brief コンストラクタ
    /// @param pin_ モーターの出力ピン
    motor(user<int> pin_) : pin(pin_) {}
    /// @brief 初期化
    void setup() const;
    /// @brief 強制停止
    void stop();

  private:
    user<int> pin; // モーター制御用ピン
  };

#ifdef __cplusplus
} // extern "C"
#endif
