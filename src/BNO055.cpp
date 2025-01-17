#include "BNO055.h"

constexpr double ti = 300;

double BNO055::convert(double data_)
{
    if ((pre - data_) > (ti))
    {
        pre = data_;
        return data_ + 360.0 * rco++;
    }
    else if ((pre - data_) < (-ti))
    {
        pre = data_;
        return data_ - 360.0 * rco--;
    }
    else
    {
        pre = data_;
        return data_ + 360.0 * rco;
    }
    return 0;
}
user<double> BNO055::get() const
{
    user<double> ret = data;
    ret.x -= defolt.x;
    ret.y -= defolt.y;
    ret.z -= defolt.z;
    ret.turn -= defolt.turn;

    // data.turn = ang.orientation.x;
    return ret;
}
user<double> BNO055::data_get() const
{
    return data;
}
sensors_event_t BNO055::getang() const
{
    return ang;
}
/*
sensors_event_t BNO055::getacc() const
{
    return acc;
}
*/
void BNO055::setup()
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

void BNO055::update()
{
    bno.getEvent(&ang, Adafruit_BNO055::VECTOR_EULER);
    /*
    bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
    if (abs(acc.acceleration.z) > 0.4)
    {
        data.z += acc.acceleration.z;
    }
    else
    {
        data.z = 0;
    }
    */
    data.x = (double)ang.orientation.y * (-1);
    data.y = (double)ang.orientation.z;
    data.turn = convert((double)ang.orientation.x);

    return;
}

void BNO055::setd(user<double> d)
{
    defolt = d;
}
