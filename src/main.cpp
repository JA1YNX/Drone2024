#include <Arduino.h>
#include "BNO055.hpp"
#include "motor.hpp"

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 58//最低回転


int64_t x,y,z,qx,qy,qz,qw,turn;//諸々値

struct user{
  int x;
  int y;
  int z;
  int turn;
};

motor m(32,33,25,26,1,2,3,4);
user u;

void setup(void)
{
    pinMode(21, INPUT_PULLUP); //SDA 21番ピンのプルアップ(念のため)
    pinMode(22, INPUT_PULLUP); //SDA 22番ピンのプルアップ(念のため)

    Serial.begin(9600);
    Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

    if (!bno.begin()) // センサの初期化
    {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    delay(1000);

  /* Display the current temperature */
    int8_t temp = bno.getTemp();
    Serial.print("Current Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    Serial.println("");

    bno.setExtCrystalUse(false);

    Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
    bno055ticker.attach_ms(BNO055interval, get_bno055_data);


  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  turn = 0;
}


void loop(void)
{
  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  m.rotate();
  m.d = 60+(z-euler.z())*50;
  m.c1 = ((x-euler.x())+(y-euler.y())*(-1))*500+turn;
  m.c2 = ((x-euler.x())*(-1)+(y-euler.y())*(-1))*500-turn;
  m.c3 = ((x-euler.x())+(y-euler.y()))*500+turn;
  m.c4 = ((x-euler.x())*(-1)+(y-euler.y()))*500-turn;

  m.c1 += (u.x+u.y*(-1))+u.turn;
  m.c2 += (u.x*(-1)+u.y*(-1))-u.turn;
  m.c3 += (u.x+u.y)+u.turn;
  m.c4 += (u.x*(-1)+u.y)-u.turn;

}