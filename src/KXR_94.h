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
    ret.x = analogRead(pin.x)/10.0-270.0;
    ret.y = analogRead(pin.y)/10.0-145.0;
    ret.z = analogRead(pin.z)/10.0-409.5;
    return ret;
}
