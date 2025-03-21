#include "jsens.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

Ticker tim;

Jsens::Jsens(int tick)
{
}

Jsens::~Jsens()
{
}

int Jsens::setup()
{
}

user<double> Jsens::getang() const
{
}

user<double> Jsens::getspd() const
{
}
