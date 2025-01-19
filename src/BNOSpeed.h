#ifndef BNOSPEED
#define BNOSPEED
#include <conf.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Ticker.h>
#include <cmath>

class BNOSpeed
{
private:
    Ticker tim;

public:
    static user<double> sensspeed;
    static user<double> sensang;
    static Adafruit_BNO055 bno;
    /// @brief 読み取った値を返す
    /// @return 読み取った値
    user<double> speed() const;
    /// @brief 加工前の値を返す
    /// @return 読み取った値
    user<double> ang() const;
    /// @brief セットアップ
    void setup();
    BNOSpeed();
    ~BNOSpeed();
};
#endif