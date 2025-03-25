#pragma once

// 使うライブラリインクルード
#include <Arduino.h>
#include "user.h"
#include "log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static constexpr double gravity = 9.80665;

    static constexpr int R_pin = 16;
    static constexpr int Y_pin = 5;
    static constexpr int G_pin = 19;
    static constexpr int PIN_ch5 = 33;

    static constexpr user<int> UC_PIN = {32, 34, 35, 23};
    static constexpr user<int> UM_PIN = {25, 26, 27, 14};
    static constexpr user<int> UC_DEF = {1526, 1526, 1110, 1470};

    static constexpr int puls = 73;      // pwm周波数
    static constexpr int bits = 12;      // ledc分解能
    static constexpr int duty_min = 360; // 最低回転
    static constexpr int duty_low = 360; // 最低回転?
    // 上に二つは正直わかんないけどこれでうまくいってる
    static constexpr int duty_max = 600; // 最高回転

    static constexpr double Max_ang = 15.0; // 機体の最大角度これを超えると強制停止

    /// pid
    static constexpr double KP = 1.0;  // 比例ゲイン
    static constexpr double KI = 0.01; // 積分ゲイン
    static constexpr double KD = 0.01; // 微分ゲイン

    inline bool check5(int in = pulseIn(PIN_ch5, HIGH, 20000), int n = 1500)
    {
        return in > n;
    }

    inline double deg2rad(double in)
    {
        static double rad = 3.14159265358979323846 / 180.0;
        return in * rad;
    } // 度をラジアンに変換する関数
    inline double rad2deg(double in)
    {
        static double rad = 180.0 / 3.14159265358979323846;
        return in * rad;
    } // ラジアンを度に変換する関数

#ifdef __cplusplus
} // extern "C"
#endif
