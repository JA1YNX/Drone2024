#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./KXR_94.h"

//#define debug_mode

#define R_pin 16
#define Y_pin 5
#define G_pin 19

//モーター制御クラスインスタンス化
motor m(25, 26, 27, 14, 1, 2, 3, 4); //(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)

//コントローラー制御用クラスインスタンス化
contloler c(user<int>{32, 35, 33, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin
//contloler c(user{33, 35, 32, 34});//ch1pin,ch2pin,ch3pin,ch4pin

KXR_94 kxr;

//セットアップ関数
void setup(void)
{
  pinMode(R_pin,OUTPUT);
  //pinMode(Y_pin,OUTPUT);
  pinMode(G_pin,OUTPUT);
  digitalWrite(R_pin,HIGH);
  digitalWrite(Y_pin,HIGH);
  //digitalWrite(R_pin,LOW);
  ledcSetup(Y_pin, puls, 8);
  ledcAttachPin(Y_pin, Y_pin);
  ledcWrite(Y_pin, 255);
  //シリアルモニタ開始
  Serial.begin(9600);
#ifdef output

  //outputが定義されていたらbluetooth開始
  bt.begin("Drone2024");
#endif

  kxr.setup(user<int>{36,0,39});

  //モーター初期化
  m.setup();//初期化
  digitalWrite(R_pin,LOW);

  //回転OFF
  m.nf = 0;//モーターの回転ON

  //初期値設定
  m.def = -6;//esc初期化

  //回転数更新
  m.rotate();//回転
  delay(500);

  //コントローラー初期化
  c.setup();

  //モーター回転ON
  m.nf = 1;//モーターの回転ON

  //ESC初期化
  m.def = 1;//esc初期化

  //回転数更新
  m.rotate();//回転

  //動作確認
  delay(500);

  //回転ストップ
  m.def = 0;//esc初期化

  //更新
  m.rotate();//回転
  
  digitalWrite(G_pin,HIGH);
  ledcWrite(Y_pin, 0);
}


void loop(void)
{
#ifdef debug_mode

  user<double> d = kxr.setreal();//プロポ入力
  //シリアルモニタにプロポの入力を出力
  Serial.print("x:");
  Serial.print(d.x);
  Serial.print(" y:");
  Serial.print(d.y);
  Serial.print(" z:");
  Serial.print(d.z);
  Serial.print(" t:");
  Serial.println(d.turn);
  

#endif //debug_mode
#ifndef debug_mode

  //プロポの入力取得
  user<double> u = c.read();//プロポ入力
  //user j = b.bno_read();
  ledcWrite(Y_pin,u.z*13);

  //処理に使う変数定義
  int x, y, z, turn; //諸々値
#ifdef output
    bt.print("{   Drone2024:");
#endif
  //if (!u.x == 0) x = j.x;
  //if (!u.y == 0) y = j.y;
  //if (!u.z == 0) z = j.z;
  //if (!u.turn == 0) turn = j.turn;

  //各モーター標準値設定
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

  //一番モーター（左上）調整値設定
//m_c1
  m.c1 += u.x;
  m.c1 -= u.y;
  m.c1 -= u.turn;
  
  //二番モーター（右上）調整値設定
//m_c2
  m.c2 -= u.x;
  m.c2 -= u.y;
  m.c2 += u.turn;
  
  //三番モーター（左下）調整値設定
//m_c3
  m.c3 += u.x;
  m.c3 += u.y;
  m.c3 += u.turn;
  
  //四番モーター（右下）調整値設定
//m_c4
  m.c4 -= u.x;
  m.c4 += u.y;
  m.c4 -= u.turn;
  user<double> j = kxr.setreal();

  if(u.x!=0&&u.turn==0)
  {
    if(j.x>0)
    {
      m.c1--;
      m.c3--;
      m.c2++;
      m.c4++;
    }
    else
    {
      m.c1++;
      m.c3++;
      m.c2--;
      m.c4--;
    }
  }


  if(u.y!=0&&u.turn==0)
  {
    if(j.y>0)
    {
      m.c1++;
      m.c2++;
      m.c3--;
      m.c4--;
    }
    else
    {
      m.c1--;
      m.c2--;
      m.c3++;
      m.c4++;
    }
  }

/*
//s_c1
  if (!u.x == 0) m.c1 += (x - j.x) * hob;
  if (!u.y == 0) m.c1 -= (y - j.y) * hob;
  if (!u.turn == 0) m.c1 += (turn - j.turn) / hob;
//s_c2
  if (!u.x == 0) m.c2 -= (x - j.x) * hob;
  if (!u.y == 0) m.c2 -= (y - j.y) * hob;
  if (!u.turn == 0) m.c2 -= (turn - j.turn) / hob;
//s_c3
  if (!u.x == 0) m.c3 += (x - j.x) * hob;
  if (!u.y == 0) m.c3 += (y - j.y) * hob;
  if (!u.turn == 0) m.c3 += (turn - j.turn) / hob;
//s_c4
  if (!u.x == 0) m.c4 -= (x - j.x) * hob;
  if (!u.y == 0) m.c4 += (y - j.y) * hob;
  if (!u.turn == 0) m.c4 -= (turn - j.turn) / hob;
*/
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
#endif //debug_mode
}
