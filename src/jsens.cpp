#include "jsens.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Ticker tim;
int freq_ms = 50;
user<double> ang;
user<double> angacc;
user<double> spd;
user<double> mag;

void callback();
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
    Log::logln("BNO055 Waiting", LogLevel::_3INFO);
    while (!bno.begin())
        Log::logln("BNO055 Waiting", LogLevel::_3INFO);
    Log::logln("BNO055 Started", LogLevel::_3INFO);
    bno.printSensorDetails();
    bno.setExtCrystalUse(true);
    tim.attach_ms(freq_ms, callback);
    return 0;
}

void callback()
{
    static user<double> res;
    static user<double> speed;
    constexpr double sikiiti = 0.20;
    static sensors_event_t acc_s, ang_s, angacc_s, mag_s;

    bno.getEvent(&acc_s, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&ang_s, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angacc_s, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&mag_s, Adafruit_BNO055::VECTOR_GRAVITY);
    res.x = acc_s.acceleration.x - (sin(deg2rad(ang_s.orientation.y)) * gravity);
    res.y = acc_s.acceleration.y + (sin(deg2rad(ang_s.orientation.z)) * gravity);
    res.z = acc_s.acceleration.z - (cos(deg2rad(ang_s.orientation.y)) * cos(deg2rad(ang_s.orientation.z)) * gravity);

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
    ang.z = 0;
    ang.turn = convert((double)ang_s.orientation.x);

    angacc.x = angacc_s.gyro.y;
    angacc.y = angacc_s.gyro.x;
    angacc.z = 0;
    angacc.turn = angacc_s.gyro.z;

    mag.x = mag_s.magnetic.x;
    mag.y = mag_s.magnetic.y;
    mag.z = mag_s.magnetic.z;
    mag.turn = 0;
}

user<double> Jsens::getang() const
{
    return ang;
}

user<double> Jsens::getspd() const
{
    return spd;
}

user<double> Jsens::getangacc() const
{
    return angacc;
}
user<double> Jsens::getmag() const
{
    return mag;
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
