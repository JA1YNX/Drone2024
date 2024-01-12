#include "motor.hpp"

void motor::rotate()
{
  if(!d == 0)
  {
      ledcWrite(ch1, abs(d+c1+dutys-1) * 1);
      ledcWrite(ch2, abs(d+c2+dutys-1) * 1);
      ledcWrite(ch3, abs(d+c3+dutys-1) * 1);
      ledcWrite(ch4, abs(d+c4+dutys-1) * 1);
  }
}
motor::motor(int pin1,int pin2,int pin3,int pin4,int cha1,int cha2,int cha3,int cha4)
{
    int ch1 = cha1;
    int ch2 = cha2;
    int ch3 = cha3;
    int ch4 = cha4;
    ledcSetup(ch1,puls,8);
    ledcSetup(ch2,puls,8);
    ledcSetup(ch3,puls,8);
    ledcSetup(ch4,puls,8);
    ledcAttachPin(pin1, ch1);
    ledcAttachPin(pin2, ch2);
    ledcAttachPin(pin3, ch3);
    ledcAttachPin(pin4, ch4);
    
    ledcWrite(ch1, abs(dutyd) * 1);//52...58~90
    ledcWrite(ch2, abs(dutyd) * 1);//58~90
    ledcWrite(ch3, abs(dutyd) * 1);//58~90
    ledcWrite(ch4, abs(dutyd) * 1);//58~90
}
