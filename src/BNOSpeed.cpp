#include <BNOSpeed.h>

void callback();
constexpr static int freq_ms = 5;

inline double dtorad(double in)
{
    constexpr static double rad = M_PI / 180;
    return in * rad;
}
double convert(double data_)
{
    constexpr static double ti = 190;
    constexpr static double mti = -ti;
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
    Log::logln("BNOSpeed Started");
    tim.attach_ms(freq_ms, callback);
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
    constexpr double deg = 60;
    static sensors_event_t acc, ang;

    bnosens.bno.getEvent(&acc, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bnosens.bno.getEvent(&ang, Adafruit_BNO055::VECTOR_EULER);
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
    bnosens.sensspeed.x = speed.x * deg;
    bnosens.sensspeed.y = speed.y * deg;
    bnosens.sensspeed.z = speed.z * deg;
    bnosens.sensang.x = (double)ang.orientation.y * (-1);
    bnosens.sensang.y = (double)ang.orientation.z;
    bnosens.sensang.turn = convert((double)ang.orientation.x);
}
