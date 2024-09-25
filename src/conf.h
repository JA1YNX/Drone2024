#pragma once

//bluetoothの出力をするかどうか
//#define output

//SerialMonitorの出力をするかどうか
#define SERIAL_out

//使うライブラリインクルード
#include <cmath>
#include <Arduino.h>
#ifdef output
#include <BluetoothSerial.h>
    //bluetooth インスタンス化
    BluetoothSerial bt;
#endif

#define puls 73//pwm周波数
#define bits 12
#define duty_min 360//最低回転58
#define duty_max 600//最高回転98

#define Max_ang 5

//とりあえずデータ入れるやつ
template <typename T>
struct user {
    T x;
    T y;
    T z;
    T turn;
};
