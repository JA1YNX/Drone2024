#ifndef BNO
#define BNO

#include "./conf.h"
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//BNO055制御用クラス
class BNO055
{
    public:
    user<int> get() const;//読み取った値
    void setup();//セットアップ
    void update();//これ実行でメンバ変数が更新される
    sensors_event_t getang() const;//センサの絶対角度を返す
    private:
    Adafruit_BNO055 bno = Adafruit_BNO055(55,0x28,&Wire);//本来の制御用クラス
    user<int> data;
    sensors_event_t ang,acc;
    int convert(int);
    int hist = 0;
    int hist2 = 0;
    user<int> defolt;
};
int BNO055::convert(int data_)
{
    int ret = 0;
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
    if ((hist-data_)>(300))
    {
        ret = data_+360*hist2;
        hist2++;
    }
    else if((hist-data_)<(-300))
    {
        ret = data_-360*hist2;
        hist2--;
    }
    else
    {
        ret = data_+360*hist2;
    }
    hist = data_;
    return ret;
}
user<int> BNO055::get() const
{
    //data.turn = ang.orientation.x;
    return data;
}
sensors_event_t BNO055::getang() const
{
    return ang;
}
void BNO055::setup()
{
    if(!bno.begin())
    {
        while(!bno.begin());
    }
    update();
    delay(500);
    defolt = get();
    return;
}

void BNO055::update()
{
    bno.getEvent(&ang,Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&acc,Adafruit_BNO055::VECTOR_LINEARACCEL);
    if(abs(acc.acceleration.z)>0.4)
    {
        data.z += acc.acceleration.z;
    }
    else{
        data.z = 0;
    }
    data.x = (int)ang.orientation.y*(-1);
    data.y = (int)ang.orientation.z;
    data.turn = convert((int)ang.orientation.x*(-1));

    data.x -= 2;
    data.y += 1;
    data.z -= 0;
    data.turn -= 0;

    return;
}
#endif
