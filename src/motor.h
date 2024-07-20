#pragma once
#include "./conf.h"

class motor { //モーターチャンネルとピン設定
    public:
        int nf;//モーターの回転ON/OFF
        int def;//もとになる数値
        int c1;//左前変更値
        int c2;//右前変更値
        int c3;//左後変更値
        int c4;//右後変更値
        void rotate();//設定反映
        motor(int pin_1, int pin_2, int pin_3, int pin_4, int cha1, int cha2, int cha3, int cha4): pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4), ch1(cha1), ch2(cha2), ch3(cha3), ch4(cha4) {}
        void setup();//初期設定
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
void motor::rotate()
{
  user<int> real = {static_cast<int>(duty_min + abs(def + c1) * nf),
              static_cast<int>(duty_min + abs(def + c2) * nf),
              static_cast<int>(duty_min + abs(def + c3) * nf),
              static_cast<int>(duty_min + abs(def + c4) * nf)};
  ledcWrite(ch1, static_cast<int>(real.x) < def ? def : real.x);
  ledcWrite(ch2, static_cast<int>(real.y) < def ? def : real.y);
  ledcWrite(ch3, static_cast<int>(real.z) < def ? def : real.z);
  ledcWrite(ch4, static_cast<int>(real.turn) < def ? def : real.turn);
  
  bt.print("  ou1:");
  bt.print(real.x);
  bt.print("  ou2:");
  bt.print(real.y);
  bt.print("  ou3:");
  bt.print(real.z);
  bt.print("  ou4:");
  bt.print(real.turn);

  bt.print("      ch1:");
  bt.print(c1);
  bt.print("  ch2:");
  bt.print(c2);
  bt.print("  ch3:");
  bt.print(c3);
  bt.print("  ch4:");
  bt.print(c4);
  bt.print("     ");
  return;
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

  ledcWrite(ch1, abs(duty_def) * 1);//52...58~90
  ledcWrite(ch2, abs(duty_def) * 1);//58~90
  ledcWrite(ch3, abs(duty_def) * 1);//58~90
  ledcWrite(ch4, abs(duty_def) * 1);//58~90
  delay(500);
  return;
}
