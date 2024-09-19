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
    user<float> get();
    void setup();
    void update();
    sensors_event_t getang();
    private:
    user<float> data;
    sensors_event_t ang,acc;
};
user<float> BNO055::get()
{
    data.turn = ang.orientation.x;
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
    if(abs(acc.acceleration.x)>0.4)
    {
        data.x += acc.acceleration.x;
    }
    else{
        data.x = 0;
    }
    if(abs(acc.acceleration.y)>0.4)
    {
        data.y += acc.acceleration.y;
    }
    else{
        data.y = 0;
    }
    if(abs(acc.acceleration.z)>0.4)
    {
        data.z += acc.acceleration.z;
    }
    else{
        data.z = 0;
    }
    data.turn = ang.orientation.x;
}
#endif
