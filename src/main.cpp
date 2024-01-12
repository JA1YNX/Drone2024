#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 58//最低回転

#define BNO055interval 10 //何ms間隔でデータを取得するか

class motor{
    public:
        int d = 0;//デフォルト
        int c1 = 0;//左前変更値
        int c2 = 0;//右前変更値
        int c3 = 0;//左後変更値
        int c4 = 0;//右後変更値
        int ch1 = 0;
        int ch2 = 0;
        int ch3 = 0;
        int ch4 = 0;
        void rotate();
        motor(int pin1,int pin2,int pin3,int pin4,int cha1,int cha2,int cha3,int cha4);
};

int64_t x,y,z,qx,qy,qz,qw,turn;//諸々値

struct user{
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


motor m(32,33,25,26,1,2,3,4);
user u;

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
}


void loop(void)
{
  x = euler.x();
  y = euler.y();
  z = euler.z();
  qx = quat.x();
  qy = quat.y();
  qz = quat.z();
  qw = quat.w();
  m.rotate();
  m.d = 60+(z-euler.z())*50.0;
  m.c1 = ((x-euler.x())+(y-euler.y())*(-1))*50.0+turn/2.0;
  m.c2 = ((x-euler.x())*(-1)+(y-euler.y())*(-1))*50.0-turn/2.0;
  m.c3 = ((x-euler.x())+(y-euler.y()))*50.0+turn/2.0;
  m.c4 = ((x-euler.x())*(-1)+(y-euler.y()))*50.0-turn/2.0;

  m.c1 += (u.x+u.y*(-1))+u.turn;
  m.c2 += (u.x*(-1)+u.y*(-1))-u.turn;
  m.c3 += (u.x+u.y)+u.turn;
  m.c4 += (u.x*(-1)+u.y)-u.turn;

  Serial.print("1");
  Serial.print(m.c1);
  Serial.print("   2");
  Serial.print(m.c2);
  Serial.print("   3");
  Serial.print(m.c3);
  Serial.print("   4");
  Serial.print(m.c4);
  Serial.println("");
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
  if(!d == 0)
  {
      ledcWrite(ch1, abs(d+c1+dutys-1) * 1);
      ledcWrite(ch2, abs(d+c2+dutys-1) * 1);
      ledcWrite(ch3, abs(d+c3+dutys-1) * 1);
      ledcWrite(ch4, abs(d+c4+dutys-1) * 1);
  }
}
motor::motor(int pin1,int pin2,int pin3,int pin4,int cha1,int cha2,int cha3,int cha4)
{
    int ch1 = cha1;
    int ch2 = cha2;
    int ch3 = cha3;
    int ch4 = cha4;
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
}
