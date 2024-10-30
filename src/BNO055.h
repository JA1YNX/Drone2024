#ifndef BNO
#define BNO

#include "./conf.h"
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// BNO055制御用クラス
class BNO055
{
public:
    user<int> get() const;          // 読み取った値
    void setup();                   // セットアップ
    void update();                  // これ実行でメンバ変数が更新される
    sensors_event_t getang() const; // センサの絶対角度を返す
private:
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); // 本来の制御用クラス
    user<int> data;
    sensors_event_t ang, acc;
    int convert(int);
    int hist = 0;
    int hist2 = 0;
    user<int> defolt;
};
#endif
