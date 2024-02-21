#include "conf.h"

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
  #ifdef output
    Serial.print(" DIR_x:");
    Serial.print(j.x);
    Serial.print(" DIR_y:");
    Serial.print(j.y);
    Serial.print(" DIR_z:");
    Serial.print(j.z);
    Serial.print(" DIR_T:");
    Serial.println(j.turn);
    #endif

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
  j.z = (j.y/2+euler.y()/2);
  j.y = (j.z/2+euler.z()/2);
  j.turn = (j.turn/2+quat.z()/2);

  //Serial.println();
}