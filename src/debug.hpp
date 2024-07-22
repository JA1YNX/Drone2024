#include "./conf.h"

void setup(void)
{
  Serial.begin(9600);
}

contloler c(user<int>{32, 35, 33, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin
void loop(void)
{
    Serial.print("x:");
    Serial.print(pulseIn(c.pin().x,HIGH,100000));
    Serial.print("y:");
    Serial.print(pulseIn(c.pin().y,HIGH,100000));
    Serial.print("z:");
    Serial.print(pulseIn(c.pin().z,HIGH,100000));
    Serial.print("t:");
    Serial.print(pulseIn(c.pin().turn,HIGH,100000));
}