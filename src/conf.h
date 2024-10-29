#pragma once

// bluetoothの出力をするかどうか
// #define output

// SerialMonitorの出力をするかどうか
#define SERIAL_out

// 使うライブラリインクルード
#include <cmath>
#include <Arduino.h>
#ifdef output
#include <BluetoothSerial.h>
// bluetooth インスタンス化
BluetoothSerial bt;
#endif

#define puls 73      // pwm周波数
#define bits 12      // ledc分解能
#define duty_min 360 // 最低回転
#define duty_low 360 // 最低回転?
// 上に二つは正直わかんないけどこれでうまくいってる
#define duty_max 600 // 最高回転

#define Max_ang 20 // 機体の最大角度これを超えると強制停止

/// pid
#define KP 0.1  // 比例ゲイン
#define KI 0.01 // 積分ゲイン
#define KD 0.05 // 微分ゲイン

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
