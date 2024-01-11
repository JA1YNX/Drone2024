#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>

#define BNO055interval 10 //何ms間隔でデータを取得するか

Ticker bno055ticker; //タイマー割り込み用のインスタンス


imu::Vector<3> gyroscope;//ジャイロ
imu::Vector<3> accelermetor;//加速度
imu::Vector<3> magnetmetor;//磁気
imu::Vector<3> euler;//センサフュージョン
imu::Quaternion quat;//クオータニオン絶対角度？
uint8_t system_, gyro, accel, mag = 0;//キャリブレーション値

void get_bno055_data(void);