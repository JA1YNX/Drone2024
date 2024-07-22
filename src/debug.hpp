#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./KXR_94.h"

contloler c(user<int>{32, 35, 33, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin
void setup(void)
{
  Serial.begin(9600);
  c.setup();
}

void loop(void)
{
    Serial.print("x:");
    Serial.print((int)pulseIn(32,HIGH));
    Serial.print("y:");
    Serial.print((int)pulseIn(35,HIGH,100000));
    Serial.print("z:");
    Serial.print((int)pulseIn(33,HIGH,100000));
    Serial.print("t:");
    Serial.println((int)pulseIn(34,HIGH,100000));
}