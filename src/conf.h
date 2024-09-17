#pragma once

//bluetoothの出力をするかどうか
#define output
//#define SERIAL

//使うライブラリインクルード
#include <cmath>
#include <Arduino.h>
#ifdef output
#include <BluetoothSerial.h>
    //bluetooth インスタンス化
    BluetoothSerial bt;
#endif

#define puls 75//pwm周波数
#define bits 12
#define duty_reset 231//初期値
#define duty_def 258//off
//#define duty_min 258//最低回転58
#define duty_min 260//最低回転58
#define duty_max 340//最高回転98

//とりあえずデータ入れるやつ
template <typename T>
struct user {
    T x;
    T y;
    T z;
    T turn;
};
