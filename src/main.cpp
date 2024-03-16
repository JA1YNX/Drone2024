#include "./conf.h"
#include "./controler.h"
#include "./motor.h"
#include "./BNO055.h"

motor m(25, 26, 27, 14, 1, 2, 3, 4); //(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)
contloler c(32, 33, 34, 35, 12, 13, user{33, 35, 32, 34});//pin1,2,3,4,5,6,ch1pin,ch2pin,ch3pin,ch4pin
bno055 b;

void setup(void)
{
  Serial.begin(9600);
#ifdef output
  bt.begin("Drone2024");
#endif
  b.bno_setup();
  m.setup();//初期化
  m.nf = 0;//モーターの回転ON
  m.def = -6;//esc初期化
  m.rotate();//回転
  delay(500);
  c.setup();
  m.nf = 1;//モーターの回転ON
  m.def = 5;//esc初期化
  m.rotate();//回転
  delay(500);
  m.def = 0;//esc初期化
  m.rotate();//回転
}


void loop(void)
{
  //user u = c.read();//プロポ入力
  user u = inter_digi::out();//プロポ入力
  Serial.print("x:");
  Serial.print(u.x);
  Serial.print(" y:");
  Serial.print(u.y);
  Serial.print(" z:");
  Serial.print(u.z);
  Serial.print(" t:");
  Serial.println(u.turn);
  user j = b.bno_read();
  int x, y, z, turn; //諸々値
  #ifdef output
    bt.print("{   Drone2024:");
    #endif
  if (!u.x == 0) x = j.x;
  if (!u.y == 0) y = j.y;
  if (!u.z == 0) z = j.z;
  if (!u.turn == 0) turn = j.turn;

  m.def = u.z-5;
  m.c1 = 0;
  m.c2 = 0;
  m.c3 = 0;
  m.c4 = 0;
//m_c1
  m.c1 -= u.x;
  m.c1 += u.y;
  m.c1 -= u.turn;
//m_c2
  m.c2 += u.x;
  m.c2 += u.y;
  m.c2 += u.turn;
//m_c3
  m.c3 -= u.x;
  m.c3 -= u.y;
  m.c3 += u.turn;
//m_c4
  m.c4 += u.x;
  m.c4 -= u.y;
  m.c4 -= u.turn;

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

  m.c1 -= 0;
  m.c2 -= 0;
  m.c3 -= 0;
  m.c4 -= 0;

  m.rotate();
#ifdef output
  bt.println("    } ");
#endif
}
