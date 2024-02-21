#pragma once


#define output
#define interrupt_on
#define interrupt_clock 1000000

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>
#ifdef output
    #include <BluetoothSerial.h>
    BluetoothSerial bt;
    #endif

#define read_ 0.01 //analogread倍率
#define hob 2.0 //ホバリング時センサ倍率

#define puls 200//pwm周波数
#define duty_def 52//初期値
#define duty_min 56//最低回転58
#define duty_max 90//最高回転98

#define BNO055interval 10 //何ms間隔でデータを取得するか

struct user { //プロポ入力
    int x;
    int y;
    int z;
    int turn;
};
