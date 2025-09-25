#pragma once

#include "conf.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Ticker.h>
#include <cmath>

/// @brief ジャイロセンサクラス
class Jsens
{
private:
protected:
public:
    /// @brief コンストラクタ
    /// @param tick 実行毎の間隔 ms
    Jsens(int tick = 50);
    ~Jsens();
    /// @brief 初期化
    /// @return エラーコード
    int setup();
    /// @brief 角度取得
    /// @return 角度
    user<double> getang() const;
    /// @brief 速度取得
    /// @return 速度
    user<double> getspd() const;
    /// @brief 角速度取得
    /// @return 角速度
    user<double> getangacc() const;
    /// @brief 重力加速取得
    /// @return 重力加速度
    user<double> getmag() const;
};
