#pragma once
#include "./conf.h"

class KXR_94
{
    public:
    void setup(user);
    user read();
    
    private:
    user pin;

};

void KXR_94::setup(user pin_in)
{
    pin = pin_in;
    pinMode(pin_in.x,INPUT);
    pinMode(pin_in.y,INPUT);
    pinMode(pin_in.z,INPUT);
    return;
}

user KXR_94::read()
{
    user ret;
    ret.x = analogRead(pin.x)/4096.0*5.0-2.5;
    ret.y = analogRead(pin.y)/4096.0*5.0-2.5;
    ret.z = analogRead(pin.z)/4096.0*5.0-2.5;
    return ret;
}
