/*
基本的なプログラムはrelease.hppと同じなのでコメントもそっち参照
*/

#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
// #include "./BNO055.h"
#include "./BNO055_tmp.h"
#include "./pid.h"

// 状態表示用LED
#define R_pin 16
#define Y_pin 5
#define G_pin 19
// ch5用読み取りピン
#define PIN_ch5 33

// モーター制御クラスインスタンス化
motor m(UM_PIN);

// コントローラー制御用クラスインスタンス化
contloler c(UC_PIN);

// BNO055
BNO055_tmp<double> sens;
int history;

// セットアップ関数
void setup(void)
{
  { // LEDとプロポのch5
    // R
    pinMode(R_pin, OUTPUT);
    digitalWrite(R_pin, HIGH);
    // Y
    ledcSetup(Y_pin, puls, 8);
    ledcAttachPin(Y_pin, Y_pin);
    ledcWrite(Y_pin, 255);
    // other
    pinMode(PIN_ch5, INPUT);
    pinMode(G_pin, OUTPUT);
  }
#ifdef SERIAL_out
  // シリアルモニタ開始
  Serial.begin(115200);
#endif

  // コントローラー初期化
  c.setup();

  // モーター初期化
  m.nf = 1;
  m.setup(); // 初期化

  // BNO055
  sens.setup();

  // LED更新
  digitalWrite(R_pin, LOW);
  delay(1000);
  m.stop();

  digitalWrite(G_pin, HIGH);

  { // 基本軸待機
    while ((pulseIn(PIN_ch5, HIGH, 20000) > 1500) && (c.read().z > 15))
      digitalWrite(R_pin, HIGH);
    while ((pulseIn(PIN_ch5, HIGH, 20000) < 1500) && (c.read().z < 2))
      digitalWrite(R_pin, LOW);
    while (c.read().z > 2)
      ;
  }

  // LED更新
  ledcWrite(Y_pin, 0);

  // 基準角度設定
  sens.update();
  sens.setd(sens.get());
  history = sens.get().turn;

  return;
}

// 強制停止用フラグ
bool flag = 0;

void loop(void)
{

  // PID用クラスと目標
  static user<int> setpoint;
  static PID_F::Pid pid_x(0);
  static PID_F::Pid pid_y(0);
  static PID_F::Pid pid_turn(0);

  // ジャイロの値
  static user<double> j;
  static user<int> u;
  static user<double> pid_res;

  sens.update();
  j = sens.get();

  flag = (abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) ||
         (pulseIn(PIN_ch5, HIGH, 20000) < 1500);

  // 強制停止
  if (pulseIn(PIN_ch5, HIGH, 20000) < 1500 || flag)
  {
    m.stop();
    digitalWrite(R_pin, HIGH);
    digitalWrite(G_pin, LOW);
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

  m.nf = 1;
  digitalWrite(R_pin, LOW);
  digitalWrite(G_pin, HIGH);

  // プロポの入力取得
  u = c.read();

  // TODO:要値調整
  ledcWrite(Y_pin, u.z * 1.2);

  // 各モーター標準値設定
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  // 目標角度設定
  { // TODO:
    setpoint.x = u.x / 1;
    setpoint.y = u.y / 1;
    setpoint.turn = u.turn / 2;
  }

  // pid
  {
    pid_res.x = pid_x.calc(j.x, setpoint.x);
    pid_res.y = pid_y.calc(j.y, setpoint.y);
    pid_res.turn = u.turn; // = pid_turn.calc(j.turn, setpoint.turn);
  }

  // set
  {
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
  }

  // 強制微調整
  m.c1 -= 0;
  m.c2 -= 0;
  m.c3 -= 0;
  m.c4 -= 0;

  // 回転数更新
  m.rotate();

#ifdef SERIAL_out
  Serial.print("     x:");
  Serial.print(u.x);
  Serial.print("  y:");
  Serial.print(u.y);
  Serial.print("  z:");
  Serial.print(u.z);
  Serial.print("  t:");
  Serial.print(u.turn);

  Serial.print("  x:");
  Serial.print(j.x);
  Serial.print("  y:");
  Serial.print(j.y);
  Serial.print("  z:");
  Serial.print(j.z);
  Serial.print("  t:");
  Serial.println(j.turn);
#endif

  return;
}
