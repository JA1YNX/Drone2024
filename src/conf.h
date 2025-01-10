#pragma once

// SerialMonitorの出力をするかどうか
#define SERIAL_out

// 使うライブラリインクルード
#include <cmath>
#include <Arduino.h>

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

constexpr int R_pin = 16;
constexpr int Y_pin = 5;
constexpr int G_pin = 19;
constexpr int PIN_ch5 = 33;

constexpr user<int> UC_PIN = {32, 34, 35, 23};
constexpr user<int> UM_PIN = {25, 26, 27, 14};
constexpr user<int> UC_DEF = {1526, 1526, 1110, 1470};

constexpr int puls = 73;      // pwm周波数
constexpr int bits = 12;      // ledc分解能
constexpr int duty_min = 360; // 最低回転
constexpr int duty_low = 360; // 最低回転?
// 上に二つは正直わかんないけどこれでうまくいってる
constexpr int duty_max = 600; // 最高回転

constexpr double Max_ang = 15.0; // 機体の最大角度これを超えると強制停止

/// pid
constexpr double KP = 1.0;  // 比例ゲイン
constexpr double KI = 0.05; // 積分ゲイン
constexpr double KD = 0.05; // 微分ゲイン
