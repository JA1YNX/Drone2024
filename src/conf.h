#pragma once

// SerialMonitorの出力をするかどうか
#define SERIAL_out

// 使うライブラリインクルード
#include <cmath>
#include <Arduino.h>

#define UC_PIN {32, 34, 35, 23}
#define UM_PIN {25, 26, 27, 14}

#define puls (73)      // pwm周波数
#define bits (12)      // ledc分解能
#define duty_min (360) // 最低回転
#define duty_low (360) // 最低回転?
// 上に二つは正直わかんないけどこれでうまくいってる
#define duty_max (600) // 最高回転

#define Max_ang (10.0) // 機体の最大角度これを超えると強制停止

/// pid
#define KP (1.0)  // 比例ゲイン
#define KI (0.05) // 積分ゲイン
#define KD (0.05)  // 微分ゲイン

// とりあえずデータ入れるやつ
// 分かり易いようにz,y,z,turnってしてるけど実際何でもいい
template <typename T>
struct user
{
    T x;
    T y;
    T z;
    T turn;
};
