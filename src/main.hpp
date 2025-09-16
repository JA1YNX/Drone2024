#include "conf.h"
#include "controller.h"
#include "motor.h"
#include "pid.h"
#include "led.h"
#include "jsens.h"

motor m(UM_PIN);

controller c(UC_PIN);

Jsens sens;

double history;

bool flag = 1;

int logout(const motor &c, const user<double> &p, const user<double> &u, const user<double> &j);

// セットアップ関数
void setup(void)
{
  Log::logset(LogLevel::_9ALL);
  setup_led();
  set_led(led_status::stop);

  Log::init();

  Log::logln("\nSetUp Started", LogLevel::_3INFO);

  m.nf = 1;
  m.setup();
  m.stop();
  c.setup();
  sens.setup();

  set_led(led_status::set);
  delay(1000);
  m.stop();

  set_led(led_status::stop);

  while (c.read().z > 2)
    ;

  set_led(led_status::wait);

  history = sens.getang().turn;
}

void loop(void)
{
  static user<double> setpoint;
  static PID_F::Pid pid_x(KP_D, KI_D, KD_D);
  static PID_F::Pid pid_y(KP_D, KI_D, KD_D);
  // static PID_F::Pid pid_turn(KP_D, KI_D, KD_D);
  static PID_F::Pid pid_turn(0, 0, 0); // note:一時的に無効化

  static user<double> j;
  static user<double> u;
  static user<double> pid_res;
  j = sens.getang();
  u = c.read();

  // 初期化処理
  {
    static bool init = 1;
    if (init)
    {
      Log::logln("Init", LogLevel::_3INFO);
      init = 0;
      pid_x.reset(j.x);
      pid_y.reset(j.y);
      pid_turn.reset(j.turn);
      setpoint.x = 0;
      setpoint.y = 0;
      setpoint.turn = 0;
    }
  }

  // TODO:要値調整
  set_led(led_status::ready, u.z * 2);

  m.nf = 1;
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  setpoint.x = u.x;
  setpoint.y = u.y;
  setpoint.turn = u.turn / 2;

  pid_res.x = pid_x.calc(j.x, setpoint.x);
  pid_res.y = pid_y.calc(j.y, setpoint.y);
  pid_res.turn = pid_turn.calc(j.turn, setpoint.turn);

  m.c1 -= pid_res.y;
  m.c2 -= pid_res.x;
  m.c3 += pid_res.x;
  m.c4 += pid_res.y;

  m.c1 += pid_res.turn;
  m.c2 -= pid_res.turn;
  m.c3 -= pid_res.turn;
  m.c4 += pid_res.turn;

  m.rotate();

  flag = (abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) || check5();

  if (flag)
  {
    set_led(led_status::stop);
    m.stop();
    // if (flag)
    while (!check5()) // プロポOFFまで待機
    {
      u = c.read();
      j = sens.getang();
      Log::log("lock0 ");
      Log::log(check5());
      Log::log(" ");
      // logout(m, pid_res, u, spd);
      logout(m, pid_res, u, j);
    }

    do
    {
      flag = 0;
      u = c.read();
      j = sens.getang();
      Log::log("lock1 ");
      Log::log(check5());
      Log::log(" ");
      // logout(m, pid_res, u, spd);
      logout(m, pid_res, u, j);
    } while ((abs(j.x) > Min_ang) || (abs(j.y) > Min_ang) ||
             ((int)u.z > Min_ang) || (abs(u.x) > Min_ang) || (abs(u.y) > Min_ang) || (abs(u.turn) > 2) ||
             check5());
    flag = 0;
    history = j.turn;
    pid_x.reset(j.x);
    pid_y.reset(j.y);
    pid_turn.reset(j.turn);
    // logout(m, pid_res, u, spd);
    logout(m, pid_res, u, j);
  }

  // logout(m, pid_res, u, spd);
  logout(m, pid_res, u, j);
}

int logout(const motor &c, const user<double> &p, const user<double> &u, const user<double> &j)
{
  Log::setdef(LogLevel::_4DEBUG);
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
  Log::setdef(LogLevel::_3INFO);
  return 0;
}
