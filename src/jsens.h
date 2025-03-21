#ifndef JSENS
#define JSENS

#include "conf.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Ticker.h>
#include <cmath>

class Jsens
{
private:
protected:
public:
    Jsens(int tick = 50);
    ~Jsens();
    int setup();
    user<double> getang() const;
    user<double> getspd() const;
};

#endif