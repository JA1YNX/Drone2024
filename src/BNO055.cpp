#include "BNO055.hpp"

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
