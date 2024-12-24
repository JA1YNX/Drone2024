#ifndef BNO
#define BNO

#include "./conf.h"
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// BNO055制御用クラス
template <typename T>
class BNO055_tmp
{
public:
    /// @brief 読み取った値を返す
    /// @return 読み取った値
    inline user<T> get() const
    {
        user<T> ret = data;
        ret.x -= defolt.x;
        ret.y -= defolt.y;
        ret.z -= defolt.z;
        ret.turn -= defolt.turn;

        // data.turn = ang.orientation.x;
        return ret;
    }
    /// @brief 加工前の値を返す
    /// @return 読み取った値
    inline user<T> data_get() const
    {
        return data;
    }
    /// @brief セットアップ
    inline void setup()
    {
        if (!bno.begin())
        {
            while (!bno.begin())
                ;
        }
        delay(500);
        update();
        return;
    }
    /// @brief 値更新
    inline void update()
    {
        bno.getEvent(&ang, Adafruit_BNO055::VECTOR_EULER);
        bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
        if (abs(acc.acceleration.z) > 0.4)
        {
            data.z += acc.acceleration.z;
        }
        else
        {
            data.z = 0;
        }
        data.x = ang.orientation.y * (-1);
        data.y = ang.orientation.z;
        data.turn = convert(ang.orientation.x * (-1));

        return;
    }
    /// @brief 加工前の角度値を返す
    /// @return sensors_event_t型
    inline sensors_event_t getang() const
    {
        return ang;
    }
    /// @brief 加工前の加速度
    /// @return sensors_event_t型
    inline sensors_event_t getacc() const
    {
        return acc;
    }
    /// @brief 初期化時点の誤差指定
    /// @param d 誤差
    inline void setd(user<T> d)
    {
        defolt = d;
    }

private:
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); // 本来の制御用クラス
    user<T> data;
    sensors_event_t ang, acc;
    inline T convert(T data_)
    {
        T ret = 0;
        /*
        if (data<180)
        {
            ret = data;
        }
        else
        {
            ret = data-360;
        }
        */
        if ((hist - data_) > (300))
        {
            ret = data_ + 360 * hist2;
            hist2++;
        }
        else if ((hist - data_) < (-300))
        {
            ret = data_ - 360 * hist2;
            hist2--;
        }
        else
        {
            ret = data_ + 360 * hist2;
        }
        hist = data_;
        return ret;
    }
    T hist = 0;
    T hist2 = 0;
    user<T> defolt;
};
#endif
