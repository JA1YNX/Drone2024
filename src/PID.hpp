#include "conf.h"
#include "controler.h"
#include "motor.h"
// #include "BNO055.h"
#include "BNO055_tmp.h"
#include "pid.h"
#include "led.h"

// モーター制御クラスインスタンス化
static motor m(UM_PIN);

// コントローラー制御用クラスインスタンス化
static contloler c(UC_PIN);

// BNO055
static BNO055_tmp<double> sens;
static int history;

// セットアップ関数
void setup(void)
{
  setup_led();
#ifdef SERIAL_out
  // シリアルモニタ開始
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

  // 基準角度設定
  sens.update();
  sens.setd(sens.get());
  history = sens.get().turn;
}

void loop(void)
{

  // PID用クラスと目標
  static user<double> setpoint;
  static PID_F::Pid pid_x(0);
  static PID_F::Pid pid_y(0);
  static PID_F::Pid pid_turn(0);

  // ジャイロの値
  static user<double> j;
  static user<int> u;
  static user<double> pid_res;

  sens.update();
  j = sens.get();
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

  pid_res.x = pid_x.calc(j.x, setpoint.x);
  pid_res.y = pid_y.calc(j.y, setpoint.y);
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

  // 強制微調整
  m.c1 -= 0;
  m.c2 -= 0;
  m.c3 -= 0;
  m.c4 -= 0;

  // 回転数更新
  m.rotate();

  // 強制停止用フラグ
  static bool flag;
  flag = (abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) ||
         (pulseIn(PIN_ch5, HIGH, 20000) < 1500);

  // 強制停止
  if (flag)
  {
    m.stop();
    set_led(::stop);

    if (flag)
      while (pulseIn(PIN_ch5, HIGH, 20000) > 1500)
        ;

    flag = 0;
    do
    {
      m.stop();
      u = c.read();
      sens.update();
      j = sens.get();
      history = sens.get().turn;
      pid_x.reset();
      pid_y.reset();
      pid_turn.reset();
      sens.setd(j);
    } while ((abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) ||
             (u.z > 2) || (u.x != 0) || (u.y != 0) || (u.turn != 0) ||
             (pulseIn(PIN_ch5, HIGH, 20000) < 1500));
  }

#ifdef SERIAL_out
  Serial.print(" ux:");
  Serial.print(u.x);
  Serial.print(" y:");
  Serial.print(u.y);
  Serial.print(" z:");
  Serial.print(u.z);
  Serial.print(" t:");
  Serial.print(u.turn);

  Serial.print(" jx:");
  Serial.print(j.x);
  Serial.print(" y:");
  Serial.print(j.y);
  Serial.print(" z:");
  Serial.print(j.z);
  Serial.print(" t:");
  Serial.println(j.turn);
#endif
}
