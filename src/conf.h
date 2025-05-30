#pragma once

// 使うライブラリインクルード
#include <Arduino.h>
#include "user.h"
#include "log.h"

/// @brief 重力加速度
constexpr double gravity = 9.80665;

/// @brief 赤LEDのピン番号
constexpr int R_pin = 16;
/// @brief 黄LEDのピン番号
constexpr int Y_pin = 5;
/// @brief 緑LEDのピン番号
constexpr int G_pin = 19;
/// @brief ch5ピン番号
constexpr int PIN_ch5 = 33;

/// @brief デフォルトコントローラーのピン番号
constexpr user<int> UC_PIN = {32, 34, 35, 23};
/// @brief デフォルトモーターのピン番号
constexpr user<int> UM_PIN = {25, 26, 27, 14};
/// @brief コントローラーデフォルト値
constexpr user<int> UC_DEF = {1526, 1526, 1110, 1470};

constexpr int puls = 73;      // pwm周波数
constexpr int bits = 12;      // ledc分解能
constexpr int duty_min = 360; // 最低回転
constexpr int duty_low = 380; // 最低回転?
// 上に二つは正直わかんないけどこれでうまくいってる
constexpr int duty_max = 600; // 最高回転

constexpr double Max_ang = 15.0; // 機体の最大角度これを超えると強制停止

/// pid
constexpr double KP_D = 0.0; // 比例ゲイン
constexpr double KI_D = 0.0; // 積分ゲイン
constexpr double KD_D = 0.0; // 微分ゲイン

constexpr double PI_D = 3.14159265358979323846;

/// ch5の状態確認
inline bool check5(int in = pulseIn(PIN_ch5, HIGH, 20000), int n = 1500)
{
    return in > n;
}

/// @brief 角度をラジアンに変換する関数
inline double deg2rad(double in)
{
    static double rad = PI_D / 180.0;
    return in * rad;
}
/// @brief ラジアンを度に変換する関数
inline double rad2deg(double in)
{
    static double rad = 180.0 / PI_D;
    return in * rad;
}
