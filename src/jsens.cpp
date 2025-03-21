#include "jsens.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Ticker tim;
int freq_ms = 50;
user<double> ang;
user<double> spd;

void callback();
inline double dtorad(double in);
double convert(double data_);

Jsens::Jsens(int tick)
{
    freq_ms = tick;
}

Jsens::~Jsens()
{
}

int Jsens::setup()
{
    while (!bno.begin())
        ;
    Log::logln("BNOSpeed Started", Log::LogLevel::_3INFO);
    tim.attach_ms(freq_ms, callback);
    return 0;
}

void callback()
{
    static user<double> res;
    static user<double> speed;
    constexpr double sikiiti = 0.20;
    static sensors_event_t acc_s, ang_s;

    bno.getEvent(&acc_s, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&ang_s, Adafruit_BNO055::VECTOR_EULER);
    res.x = acc_s.acceleration.x - (sin(dtorad(ang_s.orientation.y)) * gravity);
    res.y = acc_s.acceleration.y + (sin(dtorad(ang_s.orientation.z)) * gravity);
    res.z = acc_s.acceleration.z - (cos(dtorad(ang_s.orientation.y)) * cos(dtorad(ang_s.orientation.z)) * gravity);

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
    spd = speed;
    ang.x = (double)ang_s.orientation.y * (-1);
    ang.y = (double)ang_s.orientation.z;
    ang.turn = convert((double)ang_s.orientation.x);
}

user<double> Jsens::getang() const
{
    return ang;
}

user<double> Jsens::getspd() const
{
    return spd;
}

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
