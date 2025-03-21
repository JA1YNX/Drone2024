#include "BNO055.h"

constexpr double ti = 300;
constexpr double mti = -ti;

double BNO055::convert(double data_)
{
    static double pre_ = 0;
    static size_t rco_ = 0;
    if ((pre_ - data_) > ti)
    {
        pre_ = data_;
        return data_ + 360.0 * rco_++;
    }
    else if ((pre_ - data_) < mti)
    {
        pre_ = data_;
        return data_ - 360.0 * rco_--;
    }
    else
    {
        pre_ = data_;
        return data_ + 360.0 * rco_;
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
