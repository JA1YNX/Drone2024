#include "./conf.h"
#include "./controller.h"
#include "./motor.h"
#include "jsens.h"

controller c(user<int>{32, 35, 33, 34}); // T6J   //ch1pin,ch2pin,ch3pin,ch4pin

Jsens sens;

void setup(void)
{
  Serial.begin(115200);
  c.setup();
  sens.setup();
}

void loop(void)
{
  Serial.print("x:");
  Serial.print(sens.getang().x);
  Serial.print(" y:");
  Serial.print(sens.getang().y);
  Serial.print(" z:");
  Serial.print(sens.getang().z);
  Serial.print(" t:");
  Serial.println(sens.getang().turn);

  /*
  Serial.print("  x:");
  Serial.print(sens.speed().x);
  Serial.print(" y:");
  Serial.print(sens.speed().y);
  Serial.print(" z:");
  Serial.print(sens.speed().z);
  Serial.print(" t:");
  Serial.println(sens.speed().turn);
  */

  /*
  Serial.print("x:");
  Serial.print((int)pulseIn(32, HIGH, 20000));
  Serial.print("y:");
  Serial.print((int)pulseIn(34, HIGH, 20000));
  Serial.print("z:");
  Serial.print((int)pulseIn(35, HIGH, 20000));
  Serial.print("t:");
  Serial.println((int)pulseIn(23, HIGH, 20000));
  */
}