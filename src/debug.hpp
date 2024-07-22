#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./KXR_94.h"

void setup(void)
{
  Serial.begin(9600);
}

contloler c(user<int>{32, 35, 33, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin
void loop(void)
{
    Serial.print("x:");
    Serial.print(pulseIn(32,HIGH,100000));
    Serial.print("y:");
    Serial.print(pulseIn(35,HIGH,100000));
    Serial.print("z:");
    Serial.print(pulseIn(33,HIGH,100000));
    Serial.print("t:");
    Serial.println(pulseIn(34,HIGH,100000));
}