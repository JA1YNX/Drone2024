#include "BNO055.h"

int BNO055::convert(int data_)
{
    int ret = 0;
    /*
    if (data<180)
    {
        ret = data;
    }
    else
    {
        ret = data-360;
    }
    */
    if ((hist - data_) > (300))
    {
        ret = data_ + 360 * hist2;
        hist2++;
    }
    else if ((hist - data_) < (-300))
    {
        ret = data_ - 360 * hist2;
        hist2--;
    }
    else
    {
        ret = data_ + 360 * hist2;
    }
    hist = data_;
    return ret;
}
user<int> BNO055::get() const
{
    user<int> ret = data;
    ret.x -= defolt.x;
    ret.y -= defolt.y;
    ret.z -= defolt.z;
    ret.turn -= defolt.turn;

    // data.turn = ang.orientation.x;
    return ret;
}
user<int> BNO055::data_get() const
{
    return data;
}
sensors_event_t BNO055::getang() const
{
    return ang;
}
sensors_event_t BNO055::getacc() const
{
    return acc;
}
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
    bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
    if (abs(acc.acceleration.z) > 0.4)
    {
        data.z += acc.acceleration.z;
    }
    else
    {
        data.z = 0;
    }
    data.x = (int)ang.orientation.y * (-1);
    data.y = (int)ang.orientation.z;
    data.turn = convert((int)ang.orientation.x * (-1));

    return;
}

void BNO055::setd(user<int> d)
{
    defolt = d;
}
