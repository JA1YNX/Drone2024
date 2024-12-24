#include "BNO055.h"

template <typename T>
T BNO055<T>::convert(T data_)
{
    T ret = 0;
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
template <typename T>
user<T> BNO055<T>::get() const
{
    user<T> ret = data;
    ret.x -= defolt.x;
    ret.y -= defolt.y;
    ret.z -= defolt.z;
    ret.turn -= defolt.turn;

    // data.turn = ang.orientation.x;
    return ret;
}
template <typename T>
user<T> BNO055<T>::data_get() const
{
    return data;
}
template <typename T>
sensors_event_t BNO055<T>::getang() const
{
    return ang;
}
template <typename T>
sensors_event_t BNO055<T>::getacc() const
{
    return acc;
}
template <typename T>
void BNO055<T>::setup()
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

template <typename T>
void BNO055<T>::update()
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

template <typename T>
void BNO055<T>::setd(user<T> d)
{
    defolt = d;
}
