#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./BNO055.h"

//状態表示用LED
#define R_pin 16
#define Y_pin 5
#define G_pin 19
//ch5用読み取りピン
#define PIN_ch5 23

//モーター制御クラスインスタンス化
motor m({25, 26, 27, 14}); //(pin1,pin2,pin3,pin4)

//コントローラー制御用クラスインスタンス化
contloler c({33, 35, 32, 34});   //T6J ch1,ch2,ch3,ch4

//BNO055
BNO055 sens;
int history;

//セットアップ関数
void setup(void)
{
  //R
  pinMode(R_pin,OUTPUT);
  digitalWrite(R_pin,HIGH);
  //Y
  ledcSetup(Y_pin, puls, 8);
  ledcAttachPin(Y_pin, Y_pin);
  ledcWrite(Y_pin, 255);
  //other
  pinMode(PIN_ch5,INPUT);
  pinMode(G_pin,OUTPUT);

#ifdef SERIAL_out
  //シリアルモニタ開始
  Serial.begin(115200);
#endif
#ifdef output
  //outputが定義されていたらbluetooth開始
  bt.begin("Drone2024");
#endif

  //コントローラー初期化
  c.setup();

  //モーター初期化
  m.nf = 1;
  m.setup();//初期化

  //BNO055
  sens.setup();

  digitalWrite(R_pin,LOW);
  delay(1000);
  m.stop();

  digitalWrite(G_pin,HIGH);

  //基本軸待機
  while((pulseIn(PIN_ch5,HIGH,20000)>1500)&&(c.read().z>15))
    digitalWrite(R_pin,HIGH);
  while((pulseIn(PIN_ch5,HIGH,20000)<1500)&&(c.read().z<2))
    digitalWrite(R_pin,LOW);
  while(c.read().z>2);

  ledcWrite(Y_pin, 0);
  
  //基準角度設定
  sens.update();
  history = sens.get().turn;
}


void loop(void)
{
  //強制停止
  if(pulseIn(PIN_ch5,HIGH,20000)<1500)
  {
    m.stop();
    digitalWrite(R_pin,HIGH);
    digitalWrite(G_pin,LOW);
    while((c.read().z>2)||(pulseIn(PIN_ch5,HIGH,20000)<1500))m.stop();
    sens.update();
    history = sens.get().turn;
  }

  m.nf = 1;
  digitalWrite(R_pin,LOW);
  digitalWrite(G_pin,HIGH);

  //プロポの入力取得
  user<int> u = c.read();
  
  ledcWrite(Y_pin,u.z*7);

  #ifdef output
  bt.print("{   Drone2024:");
  #endif

  //各モーター標準値設定
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  //プロポ反映
  {
    if(u.y<0)
    {
      m.c1 -= u.y;
      m.c2 -= u.y;
    }
    else
    {
      m.c3 += u.y;
      m.c4 += u.y;
    }

    if(u.x<0)
    {
      m.c2 -= u.x;
      m.c4 -= u.x;
    }
    else
    {
      m.c1 += u.x;
      m.c3 += u.x;
    }

    if(u.turn<0)
    {
      m.c2 -= u.turn;
      m.c3 -= u.turn;
    }
    else
    {
      m.c1 += u.turn;
      m.c4 += u.turn;
    }
  }

  //ジャイロ
  sens.update();
  user<int> j = sens.get();
  {
    //BNO055
    if(u.x == 0)
    {
      m.c1-=j.x;
      m.c2+=j.x;
      m.c3-=j.x;
      m.c4+=j.x;
    }
    if(u.y == 0)
    {
      m.c1+=j.y;
      m.c2+=j.y;
      m.c3-=j.y;
      m.c4-=j.y;
    }
    if(u.turn == 0)
    {
      m.c1+=(j.turn-history);
      m.c2-=(j.turn-history);
      m.c3-=(j.turn-history);
      m.c4+=(j.turn-history);
    }
    else
    {
      history = j.turn;
    }

  }

  //強制微調整
  m.c1 -= 0;
  m.c2 -= 0;
  m.c3 -= 0;
  m.c4 -= 0;

  //回転数更新
  m.rotate();
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
  Serial.print(j.x);
  Serial.print("  y:");
  Serial.print(j.y);
  Serial.print("  z:");
  Serial.print(j.z);
  Serial.print("  t:");
  Serial.println(j.turn);
#endif
}
