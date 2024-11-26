/*
基本的なプログラムはrelease.hppと同じなのでコメントもそっち参照
*/

#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./BNO055.h"
#include "./pid.h"

// 状態表示用LED
#define R_pin 16
#define Y_pin 5
#define G_pin 19
// ch5用読み取りピン
#define PIN_ch5 33

// モーター制御クラスインスタンス化
// motor m({25, 26, 27, 14}); // pin1,pin2,pin3,pin4
motor m(UM_PIN); // pin1,pin2,pin3,pin4

// コントローラー制御用クラスインスタンス化
// contloler c({33, 35, 32, 34}); // T6J ch1,ch2,ch3,ch4
// contloler c({32, 34, 35, 23}); // T6J ch1,ch2,ch3,ch4
contloler c(UC_PIN); // T6J ch1,ch2,ch3,ch4

// BNO055
BNO055 sens;
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
#ifdef output
  // outputが定義されていたらbluetooth開始
  bt.begin("Drone2024");
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
  history = sens.get().turn;

  return;
}

// 強制停止用フラグ
bool flag = 0;

// PID用クラスと目標
user<int> setpoint;
PID_F::Pid pid_x(0);
PID_F::Pid pid_y(0);
PID_F::Pid pid_turn(0);

void loop(void)
{
  // ジャイロの値
  user<int> j;
  sens.update();
  j = sens.get();

  // 強制停止
  if (pulseIn(PIN_ch5, HIGH, 20000) < 1500 || flag)
  {
    flag = 0;
    m.stop();
    digitalWrite(R_pin, HIGH);
    digitalWrite(G_pin, LOW);
    // while((c.read().z>2)||(pulseIn(PIN_ch5,HIGH,20000)<1500))m.stop();
    user<int> stu = c.read();
    do
    {
      stu = c.read();
      sens.update();
      j = sens.get();
      history = sens.get().turn;
      m.stop();
      pid_x.reset();
      pid_y.reset();
      pid_turn.reset();
    } while ((abs(j.x) > Max_ang) || (abs(j.y) > Max_ang) || (stu.z > 2) || (stu.x != 0) || (stu.y != 0) || (stu.turn != 0) || (pulseIn(PIN_ch5, HIGH, 20000) < 1500));
  }

  m.nf = 1;
  digitalWrite(R_pin, LOW);
  digitalWrite(G_pin, HIGH);

  // プロポの入力取得
  user<int> u = c.read();
  user<int> u_r = c.data();

  // TODO:要値調整
  ledcWrite(Y_pin, u.z * 7);

#ifdef output
  bt.print("{   Drone2024:");
#endif

  // 各モーター標準値設定
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  // 目標角度設定
  { // TODO:
    setpoint.x = u_r.x / 30;
    setpoint.y = u_r.y / 30;
    setpoint.turn = u_r.turn / 30;
  }

  user<double> pid_res;
  // pid
  {
    // PID_F::pid(,,setpoint.x,,pid_res,x);
    pid_res.x = pid_x.calc(j.x, setpoint.x);
    pid_res.y = pid_y.calc(j.y, setpoint.y);
    pid_res.turn = pid_turn.calc(j.turn, setpoint.turn);
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

  user<int> jj = {0, 0, 0, 0};
#ifdef output
  bt.print("     x:");
  bt.print(j.x);
  bt.print("  y:");
  bt.print(j.y);
  bt.print("  z:");
  bt.print(j.z);
  bt.print("  t:");
  bt.print(j.turn);
  bt.println("    } ");
#endif

#ifdef SERIAL_out
  Serial.print("     x:");
  Serial.print(jj.x);
  Serial.print("  y:");
  Serial.print(jj.y);
  Serial.print("  z:");
  Serial.print(jj.z);
  Serial.print("  t:");
  Serial.print(jj.turn);

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
