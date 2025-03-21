#include "conf.h"
#include "controler.h"
#include "motor.h"
#include "pid.h"
#include "led.h"
#include "jsens.h"

motor m(UM_PIN);

contloler c(UC_PIN);

Jsens sens;

double history;

bool flag = 1;

int logout(const motor &c, const user<double> &p, const user<int> &u, const user<double> &j);

// セットアップ関数
void setup(void)
{
  Log::logset(Log::LogLevel::_9ALL);
  setup_led();
  set_led(uled_status::stop);

  Serial.begin(115200);

  Log::logln("\nSetUp Started", Log::LogLevel::_3INFO);

  m.nf = 1;
  m.setup();
  c.setup();
  sens.setup();

  set_led(uled_status::set);
  delay(1000);
  m.stop();

  set_led(uled_status::stop);

  while (c.read().z > 2)
    ;

  set_led(uled_status::wait);

  history = sens.getang().turn;
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

  j = sens.getang();
  spd = sens.getspd();
  u = c.read();

  // TODO:要値調整
  set_led(uled_status::ready, u.z * 2);

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

  if (flag)
  {
    set_led(uled_status::stop);
    m.stop();
    do
    {
      if (flag)
        while (!check5())
        {
          Log::log("locked  ");
          logout(m, pid_res, u, spd);
        }
      flag = 0;
      u = c.read();
      j = sens.getang();
      Log::log("locked  ");
      logout(m, pid_res, u, spd);
    } while ((abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) ||
             (abs(u.z) > 2) || (abs(u.x) > 2) || (abs(u.y) > 2) || (abs(u.turn) > 2) ||
             check5());
    flag = 0;
    history = j.turn;
    pid_x.reset();
    pid_y.reset();
    pid_turn.reset();
    logout(m, pid_res, u, spd);
  }

  logout(m, pid_res, u, spd);
}

int logout(const motor &c, const user<double> &p, const user<int> &u, const user<double> &j)
{
  Log::setdef(Log::LogLevel::_4DEBUG);
  Log::log("C");
  Log::log(" 1:");
  Log::log(m.c1);
  Log::log(" 2:");
  Log::log(m.c2);
  Log::log(" 3:");
  Log::log(m.c3);
  Log::log(" 4:");
  Log::log(m.c4);

  Log::log("  P");
  Log::log(" x:");
  Log::log(p.x);
  Log::log(" y:");
  Log::log(p.y);
  Log::log(" z:");
  Log::log(p.z);
  Log::log(" t:");
  Log::log(p.turn);

  Log::log("  U");
  Log::log(" x:");
  Log::log(u.x);
  Log::log(" y:");
  Log::log(u.y);
  Log::log(" z:");
  Log::log(u.z);
  Log::log(" t:");
  Log::log(u.turn);
  /*
    Log::log(" jx:");
    Log::log(j.x);
    Log::log(" y:");
    Log::log(j.y);
    Log::log(" z:");
    Log::log(j.z);
    Log::log(" t:");
    Log::log(j.turn);
  */

  Log::log("  J");
  Log::log(" x:");
  Log::log(j.x);
  Log::log(" y:");
  Log::log(j.y);
  Log::log(" z:");
  Log::log(j.z);
  Log::log(" t:");
  Log::log(j.turn);

  Log::log("\n");
  Log::setdef(Log::LogLevel::_3INFO);
  return 0;
}
