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
    /// @brief 読み取った値を返す
    /// @return 読み取った値
    user<int> get() const;
    /// @brief 加工前の値を返す
    /// @return 読み取った値
    user<int> data() const;
    /// @brief セットアップ
    void setup();
    /// @brief 値更新
    void update();
    /// @brief 加工前の角度値を返す
    /// @return sensors_event_t型
    sensors_event_t getang() const;
    /// @brief 加工前の加速度
    /// @return sensors_event_t型
    sensors_event_t getacc() const;
    /// @brief 初期化時点の誤差指定
    /// @param d 誤差
    void setd(user<int> d);

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
