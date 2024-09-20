#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
//#include "./KXR_94.h"
#include "./BNO055.h"

#define R_pin 16
#define Y_pin 5
#define G_pin 19
#define PIN_ch5 23

//モーター制御クラスインスタンス化
//motor m(25, 26, 27, 14, 1, 2, 3, 4); //(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)
motor m({25, 26, 27, 14}); //(pin1,pin2,pin3,pin4)

//コントローラー制御用クラスインスタンス化
//contloler c({32, 35, 33, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin
contloler c({33, 35, 32, 34});   //T6J   //ch1pin,ch2pin,ch3pin,ch4pin

BNO055 sens;

//セットアップ関数
void setup(void)
{
  pinMode(R_pin,OUTPUT);
  digitalWrite(R_pin,HIGH);
  ledcSetup(Y_pin, puls, 8);
  ledcAttachPin(Y_pin, Y_pin);
  ledcWrite(Y_pin, 255);
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

  //モーター初期化
  //回転OFF
  m.nf = 1;//モーターの回転ON
  m.setup();//初期化
  digitalWrite(R_pin,LOW);
  delay(1000);
  m.stop();

  //コントローラー初期化
  c.setup();

  sens.setup();

  //回転ストップ
  //m.def = 0;//esc初期化

  //更新
  //m.rotate();//回転
  
  digitalWrite(G_pin,HIGH);
  while((pulseIn(PIN_ch5,HIGH,20000)>1500)&&(c.read().z>15))
    digitalWrite(R_pin,HIGH);
  while((pulseIn(PIN_ch5,HIGH,20000)<1500)&&(c.read().z<2))
    digitalWrite(R_pin,LOW);
  while(c.read().z>2);
  ledcWrite(Y_pin, 0);
}


void loop(void)
{
  if(pulseIn(PIN_ch5,HIGH,20000)<1500)
  {
    m.stop();
    digitalWrite(R_pin,HIGH);
    digitalWrite(G_pin,LOW);
    while((c.read().z>2)||(pulseIn(PIN_ch5,HIGH,20000)<1500))m.stop();
  }
  m.nf = 1;
  digitalWrite(R_pin,LOW);
  digitalWrite(G_pin,HIGH);
  
  sens.update();

  //プロポの入力取得
  user<int> u = c.read();//プロポ入力
  //user j = b.bno_read();
  ledcWrite(Y_pin,u.z*7);

  //処理に使う変数定義
  //int x, y, z, turn; //諸々値
  #ifdef output
  bt.print("{   Drone2024:");
  #endif

  //各モーター標準値設定
  m.def = u.z;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;

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

  user<float> j = sens.get();
  /*
  user<double> j = kxr.read();
  
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
