#pragma once
#include "./conf.h"

class KXR_94
{
    public:
    void setup(user<int>);
    user<double> read();

    private:
    user<double> def;
    user<int> pin;

};

void KXR_94::setup(user<int> pin_in)
{
    pin = pin_in;
    pinMode(pin_in.x,INPUT);
    pinMode(pin_in.y,INPUT);
    pinMode(pin_in.z,INPUT);
    def = {2740,1820,4090};
    return;
}

user<double> KXR_94::read()
{
    user<double> ret;
    ret.x = (analogRead(pin.x)-def.x)/10.0;
    ret.y = (analogRead(pin.y)-def.y)/10.0;
    ret.z = (analogRead(pin.z)-def.z)/10.0;
    return ret;
}
