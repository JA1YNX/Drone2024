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
    Serial.print((double)pulseIn(32,HIGH));
    Serial.print("y:");
    Serial.print((double)pulseIn(35,HIGH));
    Serial.print("z:");
    Serial.print((double)pulseIn(33,HIGH));
    Serial.print("t:");
    Serial.println((double)pulseIn(34,HIGH));
}