#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>

#define in1 //レシーバーch1
#define in2 //レシーバーch2
#define in3 //レシーバーch3
#define in4 //レシーバーch4
#define in5 //レシーバーch5
#define in6 //レシーバーch6

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 58//最低回転

#define BNO055interval 10 //何ms間隔でデータを取得するか

class motor{//モーターチャンネルとピン設定
    public:
        int nf = 0;//モーターの回転ON/OFF
        int d = 0;//もとになる数値
        int c1 = 0;//左前変更値
        int c2 = 0;//右前変更値
        int c3 = 0;//左後変更値
        int c4 = 0;//右後変更値
        int ch1 = 0;
        int ch2 = 0;
        int ch3 = 0;
        int ch4 = 0;
        int pin1 = 0;
        int pin2 = 0;
        int pin3 = 0;
        int pin4 = 0;
        void rotate();
        motor(int pin_1,int pin_2,int pin_3,int pin_4,int cha1,int cha2,int cha3,int cha4):pin1(pin_1),pin2(pin_2),pin3(pin_3),pin4(pin_4),ch1(cha1),ch2(cha2),ch3(cha3),ch4(cha4){}
        void setup();
};

int64_t x,y,z,qx,qy,qz,qw,turn;//諸々値

struct user{//プロポ入力
  int x;
  int y;
  int z;
  int turn;
};


Ticker bno055ticker; //タイマー割り込み用のインスタンス

//Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); //ICSの名前, デフォルトアドレス, 謎
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

imu::Vector<3> gyroscope;//ジャイロ
imu::Vector<3> accelermetor;//加速度
imu::Vector<3> magnetmetor;//磁気
imu::Vector<3> euler;//センサフュージョン
imu::Quaternion quat;//クオータニオン絶対角度？
uint8_t system_, gyro, accel, mag = 0;//キャリブレーション値

void get_bno055_data(void);
void bno_setup();


motor m(32,33,25,26,1,2,3,4);//(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)
user u;//プロポ入力

void setup(void)
{
  bno_setup();
  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  turn = 0;
  pinMode(in1,INPUT);
  pinMode(in2,INPUT);
  pinMode(in3,INPUT);
  pinMode(in4,INPUT);
  pinMode(in5,INPUT);
  pinMode(in6,INPUT);

  m.setup();//初期化
  m.nf = 1;//モーターの回転ON
  m.d = -6;//esc初期化
  m.rotate();//回転
}


void loop(void)
{
  u.x = in2;
  u.y = in4;
  u.z = in3;
  u.turn = in1;

  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  m.d = 10;//+(z-euler.z())*50.0;
  m.c1 = ((x-euler.x())+(y-euler.y())*(-1.0))*2.0+turn/2.0;
  m.c2 = ((x-euler.x())*(-1.0)+(y-euler.y())*(-1.0))*2.0-turn/2.0;
  m.c3 = ((x-euler.x())+(y-euler.y()))*2.0+turn/2.0;
  m.c4 = ((x-euler.x())*(-1.0)+(y-euler.y()))*2.0-turn/2.0;

  m.c1 += (u.x+u.y*(-1.0))+u.turn;
  m.c2 += (u.x*(-1.0)+u.y*(-1.0))-u.turn;
  m.c3 += (u.x+u.y)+u.turn;
  m.c4 += (u.x*(-1.0)+u.y)-u.turn;
  m.rotate();

}


void bno_setup()
{
    pinMode(21, INPUT_PULLUP); //SDA 21番ピンのプルアップ(念のため)
    pinMode(22, INPUT_PULLUP); //SDA 22番ピンのプルアップ(念のため)

    Serial.begin(9600);
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
    bno.getCalibration(&system_, &gyro, &accel, &mag);
    //Serial.print("CALIB Sys:");
    //Serial.print(system_, DEC);
    //Serial.print(", Gy");
    //Serial.print(gyro, DEC);
    //Serial.print(", Ac");
    //Serial.print(accel, DEC);
    //Serial.print(", Mg");
    //Serial.print(mag, DEC);



    // ジャイロセンサ値の取得と表示
    gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    //Serial.print(" Gy_xyz:");
    //Serial.print(gyroscope.x());
    //Serial.print(", ");
    //Serial.print(gyroscope.y());
    //Serial.print(", ");
    //Serial.print(gyroscope.z());



    // 加速度センサ値の取得と表示
    accelermetor = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    //Serial.print(" Ac_xyz:");
    //Serial.print(accelermetor.x());
    //Serial.print(", ");
    //Serial.print(accelermetor.y());
    //Serial.print(", ");
    //Serial.print(accelermetor.z());



    // 磁力センサ値の取得と表示
    magnetmetor = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    //Serial.print(" Mg_xyz:");
    //Serial.print(magnetmetor .x());
    //Serial.print(", ");
    //Serial.print(magnetmetor .y());
    //Serial.print(", ");
    //Serial.print(magnetmetor .z());



    // センサフュージョンによる方向推定値の取得と表示
    euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    //Serial.print(" DIR_xyz:");
    //Serial.print(euler.x());
    //Serial.print(", ");
    //Serial.print(euler.y());
    //Serial.print(", ");
    //Serial.print(euler.z());



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



  //Serial.println();
}

void motor::rotate()
{
  ledcWrite(ch1, dutys+abs(d+c1) * nf);
  ledcWrite(ch2, dutys+abs(d+c2) * nf);
  ledcWrite(ch3, dutys+abs(d+c3) * nf);
  ledcWrite(ch4, dutys+abs(d+c4) * nf);
  
  Serial.print("1:");
  Serial.print(dutys+abs(d+c1) * nf);
  Serial.print("   2:");
  Serial.print(dutys+abs(d+c2) * nf);
  Serial.print("   3:");
  Serial.print(dutys+abs(d+c3) * nf);
  Serial.print("   4:");
  Serial.print(dutys+abs(d+c4) * nf);

  Serial.print("       1:");
  Serial.print(c1);
  Serial.print("   2:");
  Serial.print(c2);
  Serial.print("   3:");
  Serial.print(c3);
  Serial.print("   4:");
  Serial.print(c4);
  Serial.println("");
}
void motor::setup()
{
    ledcSetup(ch1,puls,8);
    ledcSetup(ch2,puls,8);
    ledcSetup(ch3,puls,8);
    ledcSetup(ch4,puls,8);
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