#include <BNOSpeed.h>

void callback();

inline double dtorad(double in)
{
    constexpr double rad = M_PI / 180;
    return in * rad;
}
double convert(double data_)
{
    constexpr double ti = 190;
    constexpr double mti = -ti;
    static double pre = 0;
    static long rco = 0;
    if ((pre - data_) > ti)
    {
        pre = data_;
        return data_ + 360.0 * rco++;
    }
    else if ((pre - data_) < mti)
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

Adafruit_BNO055 BNOSpeed::bno = Adafruit_BNO055(55, 0x28, &Wire);
user<double> BNOSpeed::sensspeed = {0, 0, 0, 0};
user<double> BNOSpeed::sensang = {0, 0, 0, 0};

/// @brief 読み取った値を返す
/// @return 読み取った値
user<double> BNOSpeed::speed() const
{
    return sensspeed;
}
/// @brief 加工前の値を返す
/// @return 読み取った値
user<double> BNOSpeed::ang() const
{
    return sensang;
}
/// @brief セットアップ
void BNOSpeed::setup()
{
    while (!bno.begin())
        ;
    tim.attach_ms(10, callback);
}
BNOSpeed::BNOSpeed()
{
}
BNOSpeed::~BNOSpeed()
{
}
void callback()
{
    static user<double> res;
    static user<double> speed;
    constexpr double sikiiti = 0.20;
    constexpr int freq_ms = 20;
    constexpr double deg = 60;
    static sensors_event_t acc, ang;

    BNOSpeed::bno.getEvent(&acc, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    BNOSpeed::bno.getEvent(&ang, Adafruit_BNO055::VECTOR_EULER);
    res.x = acc.acceleration.x - (sin(dtorad(ang.orientation.y)) * gravity);
    res.y = acc.acceleration.y + (sin(dtorad(ang.orientation.z)) * gravity);
    res.z = acc.acceleration.z - (cos(dtorad(ang.orientation.y)) * cos(dtorad(ang.orientation.z)) * gravity);

    if (abs(res.x) > sikiiti)
        speed.x += res.x / 1000.0 * (double)freq_ms;
    else
        speed.x = 0;

    if (abs(res.y) > sikiiti)
        speed.y += res.y / 1000.0 * (double)freq_ms;
    else
        speed.y = 0;

    if (abs(res.z) > sikiiti)
        speed.z += res.z / 1000.0 * (double)freq_ms;
    else
        speed.z = 0;
    BNOSpeed::sensspeed.x = speed.x * deg;
    BNOSpeed::sensspeed.y = speed.y * deg;
    BNOSpeed::sensspeed.z = speed.z * deg;
    BNOSpeed::sensang.x = (double)ang.orientation.y * (-1);
    BNOSpeed::sensang.y = (double)ang.orientation.z;
    BNOSpeed::sensang.turn = convert((double)ang.orientation.x);
}