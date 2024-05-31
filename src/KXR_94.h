#pragma once
#include "./conf.h"

class KXR_94
{
    public:
    void setup(user<int>);
    user<double> read();
    
    private:
    user<int> pin;

};

void KXR_94::setup(user<int> pin_in)
{
    pin = pin_in;
    pinMode(pin_in.x,INPUT);
    pinMode(pin_in.y,INPUT);
    pinMode(pin_in.z,INPUT);
    return;
}

user<double> KXR_94::read()
{
    user<double> ret;
    ret.x = analogRead(pin.x)/4096.0*5.0-2.5;
    ret.y = analogRead(pin.y)/4096.0*5.0-1.0;
    ret.z = analogRead(pin.z)/4096.0*5.0-5.0;
    return ret;
}
