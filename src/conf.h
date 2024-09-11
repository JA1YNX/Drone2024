#pragma once

//bluetoothの出力をするかどうか
#define output
#define bt Serial
//割り込み使うか
//#define interrupt_on

//割り込み周期
//#define interrupt_clock 1000000

//使うライブラリインクルード
#include <cmath>
#include <Arduino.h>
#ifdef output
#include <BluetoothSerial.h>
    //bluetooth インスタンス化
//    BluetoothSerial bt;
#endif

//#define hob 2.0 //ホバリング時センサ倍率
/*
//#define puls 200//pwm周波数
#define puls 75//pwm周波数
//#define duty_def 52//初期値
#define duty_def 19//初期値
//#define duty_min 56//最低回転58
#define duty_min 19//最低回転58
#define duty_max 36//最高回転98
*/

#define puls 75//pwm周波数
#define bits 12
#define duty_reset 231//初期値
//#define duty_def 258//off
#define duty_def 285//off
//#define duty_min 260//最低回転58
#define duty_min 258//最低回転58
#define duty_max 340//最高回転98

//#define BNO055interval 2 //何ms間隔でデータを取得するか

//とりあえずデータ入れるやつ
template <typename T>
struct user {
    T x;
    T y;
    T z;
    T turn;
};
