#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Ticker.h>
#include <BluetoothSerial.h>

#define read_ 0.01 //analogread倍率

#define puls 200//pwm周波数
#define dutyd 52//初期値
#define dutys 58//最低回転
#define p_max 90//最高回転

#define BNO055interval 10 //何ms間隔でデータを取得するか

struct user{//プロポ入力
  double x;
  double y;
  double z;
  double turn;
};

class motor{//モーターチャンネルとピン設定
    public:
        int nf;//モーターの回転ON/OFF
        int d;//もとになる数値
        int c1;//左前変更値
        int c2;//右前変更値
        int c3;//左後変更値
        int c4;//右後変更値
        int ch1;
        int ch2;
        int ch3;
        int ch4;
        int pin1;
        int pin2;
        int pin3;
        int pin4;
        void rotate();
        motor(int pin_1,int pin_2,int pin_3,int pin_4,int cha1,int cha2,int cha3,int cha4):pin1(pin_1),pin2(pin_2),pin3(pin_3),pin4(pin_4),ch1(cha1),ch2(cha2),ch3(cha3),ch4(cha4){}
        void setup();
};

class contloler{
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
        contloler(int pin1,int pin2,int pin3,int pin4,int pin5,int pin6,user set_);
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


motor m(25,26,27,14,1,2,3,4);//(pin1,pin2,pin3,pin4,ch1,ch2,ch3,ch4)
user u;//プロポ入力
contloler c(32,33,34,35,12,13,user{35,33,34,32});

void setup(void)
{
  bno_setup();
  //c.setup();
  bt.begin("Drone2024");

  m.setup();//初期化
  m.nf = 1;//モーターの回転ON
  m.d = -6;//esc初期化
  m.rotate();//回転
}


void loop(void)
{
  int64_t x,y,z,turn;//諸々値
  bt.print("Drone2024:");
  u = c.read(); 

  x = euler.x();
  y = euler.y();
  z = euler.z();
  m.d = 10;//+(z-euler.z())*50.0;
  m.c1 = ((x-euler.x())+(y-euler.y())*(-1.0))*2.0+turn/2.0;
  m.c2 = ((x-euler.x())*(-1.0)+(y-euler.y())*(-1.0))*2.0-turn/2.0;
  m.c3 = ((x-euler.x())+(y-euler.y()))*2.0+turn/2.0;
  m.c4 = ((x-euler.x())*(-1.0)+(y-euler.y()))*2.0-turn/2.0;

  m.c1 += ((u.x+u.y*(-1.0))+u.turn);
  m.c2 += ((u.x*(-1.0)+u.y*(-1.0))-u.turn);
  m.c3 += ((u.x+u.y)+u.turn);
  m.c4 += ((u.x*(-1.0)+u.y)-u.turn);
  m.rotate();
  bt.println("     ");

}
contloler::contloler(int pin1,int pin2,int pin3,int pin4,int pin5,int pin6,user set_)
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
  pinMode(pin_in1,OUTPUT);
  pinMode(pin_in2,OUTPUT);
  pinMode(pin_in3,OUTPUT);
  pinMode(pin_in4,OUTPUT);
  pinMode(pin_in5,OUTPUT);
  pinMode(pin_in6,OUTPUT);
}

user contloler::read()
{
  double x = (analogRead(set.x)-1.5)*read_;
  double y = (analogRead(set.y)-1.5)*read_;
  double z = (analogRead(set.z)-0.5)*read_*10;
  double turn = (analogRead(set.turn)-1.5)*read_;
  bt.print(" cx:");
  bt.print(x);
  bt.print(" cy:");
  bt.print(y);
  bt.print(" cz:");
  bt.print(z);
  bt.print(" ct:");
  bt.print(turn);
  bt.print("   ");
  return user{x,y,z,turn};
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
  
  bt.print("  ou1:");
  bt.print(dutys+abs(d+c1) * nf);
  bt.print("  ou2:");
  bt.print(dutys+abs(d+c2) * nf);
  bt.print("  ou3:");
  bt.print(dutys+abs(d+c3) * nf);
  bt.print("  ou4:");
  bt.print(dutys+abs(d+c4) * nf);

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