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
volatile int logno = 0;
int logout(const motor &c, const user<double> &p, const user<double> &u, const user<double> &j);
int logout(const user<double> &p, const char *tag = "");

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
  Log::logln("\n", LogLevel::_3INFO);
  Log::logln(logno++, LogLevel::_3INFO);
  sens.setup();
  Log::logln("\n", LogLevel::_3INFO);
  Log::logln(logno++, LogLevel::_3INFO);

  set_led(led_status::set);
  delay(1000);
  m.stop();

  set_led(led_status::stop);

  while (c.read().z > 2)
    ;

  set_led(led_status::wait);

  history = sens.getang().turn;
  Log::logln("\nInit Done", LogLevel::_3INFO);
}

void loop(void)
{
  while (0) // テストループ
  {
    // logout(c.data(), "U      ");
    // logout(sens.getang(), "ANG    ");
    // logout(sens.getspd(), "SPD    ");
    logout(sens.getangacc(), "ANGACC ");
    // logout(sens.getmag(), "MAG    ");
    // Log::log("\n");
    // delay(500);
  }

  static user<double> setpoint;
  // static PID_F::Pid pid_x(0);
  // static PID_F::Pid pid_y(0);
  // static PID_F::Pid pid_turn(0); // note:一時的に無効化
  static PID_F::AngleController pid_x;
  static PID_F::AngleController pid_y;
  static PID_F::AngleController pid_turn;

  static user<double> j_ang;
  static user<double> j_angacc;
  static user<double> u;
  static user<double> pid_res;
  j_ang = sens.getang();
  j_angacc = sens.getangacc();
  u = c.read();

  // 初期化処理
  {
    static bool init = 1;
    if (init)
    {
      Log::logln("Init", LogLevel::_3INFO);
      init = 0;
      // pid_x.reset(j.x);
      // pid_y.reset(j.y);
      // pid_turn.reset(j.turn);
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

  // pid_res.x = pid_x.calc(j.x, setpoint.x, KP_D, KI_D, KD_D);
  // pid_res.y = pid_y.calc(j.y, setpoint.y, KP_D, KI_D, KD_D);
  // pid_res.turn = pid_turn.calc(j.turn, setpoint.turn, KP_D, KI_D, KD_D);
  pid_res.x = pid_x.calc(j_ang.x, setpoint.x, j_angacc.x, KP_D, KI_D, KD_D, KP_ACC_D, KI_ACC_D, KD_ACC_D);
  pid_res.y = pid_y.calc(j_ang.y, setpoint.y, j_angacc.y, KP_D, KI_D, KD_D, KP_ACC_D, KI_ACC_D, KD_ACC_D);
  pid_res.turn = pid_turn.calc(j_ang.turn, j_angacc.turn, setpoint.turn, KP_D, KI_D, KD_D, KP_ACC_D, KI_ACC_D, KD_ACC_D);

  m.c1 -= pid_res.y;
  m.c2 -= pid_res.x;
  m.c3 += pid_res.x;
  m.c4 += pid_res.y;

  m.c1 += pid_res.turn;
  m.c2 -= pid_res.turn;
  m.c3 -= pid_res.turn;
  m.c4 += pid_res.turn;

  flag = (abs(j_ang.x) > Max_ang) || (abs(j_ang.y) > Max_ang) || check5();

  if (flag)
  {
    set_led(led_status::stop);
    m.stop();
    // if (flag)
    while (!check5()) // プロポOFFまで待機
    {
      u = c.read();
      j_ang = sens.getang();
      j_angacc = sens.getangacc();
      Log::log("lock0 ");
      Log::log(check5());
      Log::log(" ");
      logout(m, pid_res, u, j_ang);
    }

    do
    {
      flag = 0;
      u = c.read();
      j_ang = sens.getang();
      j_angacc = sens.getangacc();
      Log::log("lock1 ");
      Log::log(check5());
      Log::log(" ");
      logout(m, pid_res, u, j_ang);
    } while ((abs(j_ang.x) > Min_ang) || (abs(j_ang.y) > Min_ang) ||
             ((int)u.z > Min_ang) || (abs(u.x) > Min_ang) || (abs(u.y) > Min_ang) || (abs(u.turn) > 2) ||
             check5());
    flag = 0;
    history = j_ang.turn;
    // pid_x.reset(j.x);
    // pid_y.reset(j.y);
    // pid_turn.reset(j.turn);
    logout(m, pid_res, u, j_ang);
  }

  m.rotate();

  logout(m, pid_res, u, j_ang);
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
int logout(const user<double> &p, const char *tag)
{
  Log::setdef(LogLevel::_4DEBUG);
  Log::log(tag);

  Log::log(" x:");
  Log::log(p.x);
  Log::log(" y:");
  Log::log(p.y);
  Log::log(" z:");
  Log::log(p.z);
  Log::log(" t:");
  Log::log(p.turn);
  Log::log("\n");
  Log::setdef(LogLevel::_3INFO);
  return 0;
}