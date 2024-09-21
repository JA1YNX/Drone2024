#ifndef BNO
#define BNO

#include "./conf.h"
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO055
{
    public:
    Adafruit_BNO055 bno = Adafruit_BNO055(55,0x28,&Wire);
    user<int> get();
    void setup();
    void update();
    sensors_event_t getang();
    private:
    user<int> data;
    sensors_event_t ang,acc;
    int convert(int);
};
int BNO055::convert(int data)
{
    int ret = 0;
    if (data<180)
    {
        ret = data;
    }
    else
    {
        ret = data-360;
    }
    return ret;
}
user<int> BNO055::get()
{
    //data.turn = ang.orientation.x;
    return data;
}
sensors_event_t BNO055::getang()
{
    return ang;
}
void BNO055::setup()
{
    if(!bno.begin())
    {
        while(!bno.begin());
    }
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
    data.turn = convert((int)ang.orientation.x);
}
#endif
