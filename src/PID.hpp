#include "conf.h"
#include "controler.h"
#include "motor.h"
#include "BNO055.h"
#include "pid.h"
#include "led.h"
#include "BNOSpeed.h"

static motor m(UM_PIN);

static contloler c(UC_PIN);

static BNOSpeed sens;
// static BNO055 sens;
volatile static double history;

volatile static bool flag = 1;

// セットアップ関数
void setup(void)
{
  setup_led();
  set_led(::stop);
#ifdef SERIAL_out
  Serial.begin(115200);
#endif

  m.nf = 1;
  m.setup();
  c.setup();
  sens.setup();

  set_led(::set);
  delay(1000);
  m.stop();

  set_led(::stop);

  while (c.read().z > 2)
    ;

  set_led(::wait);

  // sens.update();
  // sens.setd(sens.get());
  history = sens.ang().turn;
  // history = sens.get().turn;
}

void loop(void)
{
  static user<double> setpoint;
  static PID_F::Pid pid_x(0);
  static PID_F::Pid pid_y(0);
  static PID_F::Pid pid_turn(0);

  static user<double> j;
  static user<double> spd;
  static user<int> u;
  static user<double> pid_res;

  // sens.update();
  // j = sens.get();
  j = sens.ang();
  spd = sens.speed();
  u = c.read();

  // TODO:要値調整
  set_led(::ready, u.z * 2);

  m.nf = 1;
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  // TODO:
  setpoint.x = u.x / 1;
  setpoint.y = u.y / 1;
  setpoint.turn = u.turn / 2;

  // pid_res.x = pid_x.calc(j.x, setpoint.x);
  // pid_res.y = pid_y.calc(j.y, setpoint.y);
  pid_res.x = pid_x.calc(spd.x, setpoint.x);
  pid_res.y = pid_y.calc(spd.y, setpoint.y);
  pid_res.turn = u.turn; // = pid_turn.calc(j.turn, setpoint.turn);

  m.c1 += pid_res.x;
  m.c2 -= pid_res.x;
  m.c3 += pid_res.x;
  m.c4 -= pid_res.x;

  m.c1 -= pid_res.y;
  m.c2 -= pid_res.y;
  m.c3 += pid_res.y;
  m.c4 += pid_res.y;

  m.c1 += pid_res.turn;
  m.c2 -= pid_res.turn;
  m.c3 -= pid_res.turn;
  m.c4 += pid_res.turn;

  m.c1 -= 0;
  m.c2 -= 0;
  m.c3 -= 0;
  m.c4 -= 0;

  m.rotate();

  flag = (abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) || check5();
  flag = 0;
  if (flag)
  {
    set_led(::stop);
    do
    {
      m.stop();
      if (flag)
        while (!check5())
          ;
      flag = 0;
      u = c.read();
      // sens.update();
      // j = sens.get();
      j = sens.ang();
      history = j.turn;
      pid_x.reset();
      pid_y.reset();
      pid_turn.reset();
      // sens.setd(j);
    } while ((abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) ||
             (u.z > 2) || (u.x != 0) || (u.y != 0) || (u.turn != 0) ||
             check5());
  }

#ifdef SERIAL_out
  Serial.print("c1:");
  Serial.print(m.c1);
  Serial.print(" 2:");
  Serial.print(m.c2);
  Serial.print(" 3:");
  Serial.print(m.c3);
  Serial.print(" 4:");
  Serial.print(m.c4);

  Serial.printf("%c ", ' ');

  Serial.print(" px:");
  Serial.print(pid_res.x);
  Serial.print(" y:");
  Serial.print(pid_res.y);
  Serial.print(" z:");
  Serial.print(pid_res.z);
  Serial.print(" t:");
  Serial.print(pid_res.turn);

  Serial.printf("%c ", ' ');

  Serial.print(" ux:");
  Serial.print(u.x);
  Serial.print(" y:");
  Serial.print(u.y);
  Serial.print(" z:");
  Serial.print(u.z);
  Serial.print(" t:");
  Serial.print(u.turn);
  /*
    Serial.printf("%c ", ' ');

    Serial.print(" jx:");
    Serial.print(j.x);
    Serial.print(" y:");
    Serial.print(j.y);
    Serial.print(" z:");
    Serial.print(j.z);
    Serial.print(" t:");
    Serial.println(j.turn);
  */
  Serial.printf("%c ", ' ');

  Serial.print(" jx:");
  Serial.print(spd.x);
  Serial.print(" y:");
  Serial.print(spd.y);
  Serial.print(" z:");
  Serial.print(spd.z);
  Serial.print(" t:");
  Serial.println(spd.turn);
#endif
}
