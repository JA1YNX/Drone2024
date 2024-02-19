#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>
#include <BluetoothSerial.h>

#define read_ 0.01 //analogread倍率
#define hob 2.0 //ホバリング時センサ倍率

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 56//最低回転58
#define p_max 90//最高回転98

#define BNO055interval 10 //何ms間隔でデータを取得するか

struct user { //プロポ入力
    int x;
    int y;
    int z;
    int turn;
};
