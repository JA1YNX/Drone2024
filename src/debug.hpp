#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "BNO055.h"

contloler c(user<int>{32, 35, 33, 34}); // T6J   //ch1pin,ch2pin,ch3pin,ch4pin

// #define sens bnosens
#define ang get
BNO055 sens;

void setup(void)
{
  Serial.begin(115200);
  c.setup();
  sens.setup();
}

void loop(void)
{
  Serial.print("x:");
  Serial.print(sens.ang().x);
  Serial.print(" y:");
  Serial.print(sens.ang().y);
  Serial.print(" z:");
  Serial.print(sens.ang().z);
  Serial.print(" t:");
  Serial.println(sens.ang().turn);
  sens.update();

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