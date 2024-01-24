#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>
#include <BluetoothSerial.h>

#define mode 1//0:ハード1:ソフト
#define mode_clock 1000//esp32のカウント数

#define read_ 0.01 //analogread倍率
#define hob 2.0 //ホバリング時センサ倍率

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 56//最低回転58
#define p_max 90//最高回転98

#define BNO055interval 5 //何ms間隔でデータを取得するか

struct user { //プロポ入力
  int x;
  int y;
  int z;
  int turn;
};

class motor { //モーターチャンネルとピン設定
  public:
    int nf;//モーターの回転ON/OFF
    int d;//もとになる数値
    int c1;//左前変更値
    int c2;//右前変更値
    int c3;//左後変更値
    int c4;//右後変更値
    void rotate();
    motor(int pin_1, int pin_2, int pin_3, int pin_4, int cha1, int cha2, int cha3, int cha4): pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4), ch1(cha1), ch2(cha2), ch3(cha3), ch4(cha4) {}
    void setup();
  private:
    int ch1;
    int ch2;
    int ch3;
    int ch4;
    int pin1;
    int pin2;
    int pin3;
    int pin4;
};

class contloler {
  public:
    int pin_in1;
    int pin_in2;
    int pin_in3;
    int pin_in4;
    int pin_in5;
    int pin_in6;
    user set;
    void setup();
    user read();
    contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_);
  private:
    int x;
    int y;
    int z;
    int turn;    
};

Ticker bno055ticker; //タイマー割り込み用のインスタンス

//Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); //ICSの名前, デフォルトアドレス, 謎
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

BluetoothSerial bt;

imu::Vector<3> gyroscope;//ジャイロ
imu::Vector<3> accelermetor;//加速度
imu::Vector<3> magnetmetor;//磁気
imu::Vector<3> euler;//センサフュージョン
imu::Quaternion quat;//クオータニオン絶対角度？
uint8_t system_, gyro, accel, mag = 0;//キャリブレーション値

void get_bno055_data(void);
void bno_setup();

void mode_setup();
void mode_read();

motor m(25, 26, 27, 14, 1, 2, 3, 4); //(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)
user u;//プロポ入力
user ud;//標準
user j;//BNO055値
contloler c(32, 33, 34, 35, 12, 13, user{33, 35, 32, 34});//pin1,2,3,4,5,6,ch1pin,ch2pin,ch3pin,ch4pin
hw_timer_t * timer = NULL;
volatile user mode_timer;
volatile user mode_set;
volatile bool mode_chx;
volatile bool mode_chy;
volatile bool mode_chz;
volatile bool mode_chturn;

void setup(void)
{
  Serial.begin(115200);
  bt.begin("Drone2024");

  bno_setup();
  c.setup();
  if(mode)mode_setup();
  delay(5000);
  ud = user{analogRead(33),analogRead(35),analogRead(32),analogRead(34)};
  if(mode)ud = user{mode_set.x,mode_set.y,mode_set.z,mode_set.turn};
  m.setup();//初期化
  m.nf = 0;//モーターの回転ON
  m.d = -6;//esc初期化
  m.rotate();//回転
  delay(500);
  m.nf = 1;//モーターの回転ON
  m.d = 5;//esc初期化
  m.rotate();//回転
  delay(1000);
  m.d = 0;//esc初期化
  m.rotate();//回転
}


void loop(void)
{
  int x, y, z, turn; //諸々値

  bt.print("{   Drone2024:");

  u = c.read();
  if (!u.x == 0) x = j.x;
  if (!u.y == 0) y = j.y;
  if (!u.z == 0) z = j.z;
  if (!u.turn == 0) turn = j.turn;

  m.d = u.z-5;
  m.c1 = 100;
  m.c2 = 100;
  m.c3 = 100;
  m.c4 = 100;
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

  m.c1 -= 100;
  m.c2 -= 100;
  m.c3 -= 100;
  m.c4 -= 100;

  m.rotate();
  bt.println("    } ");

}

void mode_setup()
{
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer,&mode_read,true);
  timerAlarmWrite(timer,mode_clock,true);
  timerAlarmEnable(timer);
}
void mode_read()
{
  bool x = 0,y = 0,z = 0,turn = 0;
  //測る
  x = digitalRead(c.set.x);
  y = digitalRead(c.set.y);
  z = digitalRead(c.set.z);
  turn = digitalRead(c.set.turn);
  //足す
  if(x == 1)
  {
    mode_timer.x++;
    mode_chx = 1;
  }
  if(y == 1)
  {
    mode_timer.y++;
    mode_chy = 1;
  }
  if(z == 1)
  {
    mode_timer.z++;
    mode_chy = 1;
  }
  if(turn == 1)
  {
    mode_timer.turn++;
    mode_chturn = 1;
  }

  //リセット
  if(x == 0)
  {
    if(mode_chx == 1)
    {
      mode_set.x = mode_timer.x;
    }
    mode_timer.x = 0;
    mode_chx = 0;
  }
  if(y == 0)
  {
    if(mode_chy == 1)
    {
      mode_set.y = mode_timer.y;
    }
    mode_timer.y = 0;
    mode_chy = 0;
  }
  if(z == 0)
  {
    if(mode_chz == 1)
    {
      mode_set.z = mode_timer.z;
    }
    mode_timer.z = 0;
    mode_chz = 0;
  }
  if(turn == 0)
  {
    if(mode_chturn == 1)
    {
      mode_set.turn = mode_timer.turn;
    }
    mode_timer.turn = 0;
    mode_chturn = 0;
  } 
}

contloler::contloler(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, user set_)
{
  pin_in1 = pin1;
  pin_in2 = pin2;
  pin_in3 = pin3;
  pin_in4 = pin4;
  pin_in5 = pin5;
  pin_in6 = pin6;
  set = set_;
}
void contloler::setup()
{
  pinMode(pin_in1, INPUT);
  pinMode(pin_in2, INPUT);
  pinMode(pin_in3, INPUT);
  pinMode(pin_in4, INPUT);
  pinMode(pin_in5, INPUT);
  pinMode(pin_in6, INPUT);
  x = 0;
  y = 0;
  z = 0;
  turn = 0;
}

user contloler::read()
{
  x = (x/2+((!mode?analogRead(set.x):mode_set.x-ud.x) * read_)*5);
  y = (y/2+((!mode?analogRead(set.y):mode_set.y-ud.y) * read_)*5);
  z = (z/2+((!mode?analogRead(set.z):mode_set.z-ud.z) * read_)*5);
  turn = (turn/2+((!mode?analogRead(set.turn):mode_set.turn-ud.turn) * read_)*5);
  /*
  int x = (analogRead(set.x) - ud.x) * read_*10;
  int y = (analogRead(set.y) - ud.y) * read_*10;
  int z = (analogRead(set.z) - ud.z) * read_*10;
  int turn = (analogRead(set.turn) - ud.turn) * read_*10;
  */
  bt.print("   cx:");
  bt.print(x);
  bt.print("   cy:");
  bt.print(y);
  bt.print("   cz:");
  bt.print(z);
  bt.print("   ct:");
  bt.print(turn);
  bt.print("     ");
  return user{x, y, z, turn};
}

void bno_setup()
{
  pinMode(21, INPUT_PULLUP); //SDA 21番ピンのプルアップ(念のため)
  pinMode(22, INPUT_PULLUP); //SDA 22番ピンのプルアップ(念のため)

  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  if (!bno.begin()) // センサの初期化
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(false);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
  bno055ticker.attach_ms(BNO055interval, get_bno055_data);

  j.x = 0;
  j.y = 0;
  j.z = 0;
  j.turn = 0;

}

void get_bno055_data(void)
{
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2


  // キャリブレーションのステータスの取得と表示
  //bno.getCalibration(&system_, &gyro, &accel, &mag);
  //Serial.print("CALIB Sys:");
  //Serial.print(system_, DEC);
  //Serial.print(", Gy");
  //Serial.print(gyro, DEC);
  //Serial.print(", Ac");
  //Serial.print(accel, DEC);
  //Serial.print(", Mg");
  //Serial.print(mag, DEC);



  // ジャイロセンサ値の取得と表示
  //gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  //Serial.print(" Gy_xyz:");
  //Serial.print(gyroscope.x());
  //Serial.print(", ");
  //Serial.print(gyroscope.y());
  //Serial.print(", ");
  //Serial.print(gyroscope.z());



  // 加速度センサ値の取得と表示
  //accelermetor = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  //Serial.print(" Ac_xyz:");
  //Serial.print(accelermetor.x());
  //Serial.print(", ");
  //Serial.print(accelermetor.y());
  //Serial.print(", ");
  //Serial.print(accelermetor.z());



  // 磁力センサ値の取得と表示
  //magnetmetor = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  //Serial.print(" Mg_xyz:");
  //Serial.print(magnetmetor .x());
  //Serial.print(", ");
  //Serial.print(magnetmetor .y());
  //Serial.print(", ");
  //Serial.print(magnetmetor .z());



  // センサフュージョンによる方向推定値の取得と表示
  euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Serial.print(" DIR_x:");
  Serial.print(j.x);
  Serial.print(" DIR_y:");
  Serial.print(j.y);
  Serial.print(" DIR_z:");
  Serial.print(j.x);
  Serial.print(" DIR_T:");
  Serial.println(j.turn);

  // センサフュージョンの方向推定値のクオータニオン
  quat = bno.getQuat();
  //Serial.print("qW: ");
  //Serial.print(quat.w(), 4);
  //Serial.print(" qX: ");
  //Serial.print(quat.x(), 4);
  //Serial.print(" qY: ");
  //Serial.print(quat.y(), 4);
  //Serial.print(" qZ: ");
  //Serial.print(quat.z(), 4);
  //Serial.print("\t\t");

  j.x = (j.x/2+euler.x()/2);
  j.y = (j.y/2+euler.y()/2);
  j.z = (j.z/2+euler.z()/2);
  j.turn = (j.turn/2+quat.z()/2);

  //Serial.println();
}

void motor::rotate()
{
  ledcWrite(ch1, dutys + abs(d + c1) * nf);
  ledcWrite(ch2, dutys + abs(d + c2) * nf);
  ledcWrite(ch3, dutys + abs(d + c3) * nf);
  ledcWrite(ch4, dutys + abs(d + c4) * nf);

  bt.print("  ou1:");
  bt.print(dutys + abs(d + c1) * nf);
  bt.print("  ou2:");
  bt.print(dutys + abs(d + c2) * nf);
  bt.print("  ou3:");
  bt.print(dutys + abs(d + c3) * nf);
  bt.print("  ou4:");
  bt.print(dutys + abs(d + c4) * nf);

  bt.print("      ch1:");
  bt.print(c1);
  bt.print("  ch2:");
  bt.print(c2);
  bt.print("  ch3:");
  bt.print(c3);
  bt.print("  ch4:");
  bt.print(c4);
  bt.print("     ");
}
void motor::setup()
{
  ledcSetup(ch1, puls, 8);
  ledcSetup(ch2, puls, 8);
  ledcSetup(ch3, puls, 8);
  ledcSetup(ch4, puls, 8);
  ledcAttachPin(pin1, ch1);
  ledcAttachPin(pin2, ch2);
  ledcAttachPin(pin3, ch3);
  ledcAttachPin(pin4, ch4);

  ledcWrite(ch1, abs(dutyd) * 1);//52...58~90
  ledcWrite(ch2, abs(dutyd) * 1);//58~90
  ledcWrite(ch3, abs(dutyd) * 1);//58~90
  ledcWrite(ch4, abs(dutyd) * 1);//58~90
  delay(500);
}