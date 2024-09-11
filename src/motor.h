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
        //motor(int pin_1, int pin_2, int pin_3, int pin_4): pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4){}
        motor(user<int> pin_): pin(pin_){}
        void setup();//初期設定
        void stop();
    private:
        user<int> pin;
};
void motor::stop()
{
  nf = 0;
  rotate();
}
void motor::rotate()
{
  ledcWrite(1, c1 < 0 ? (def+duty_min)*nf : (def+c1)*nf);
  ledcWrite(2, c2 < 0 ? (def+duty_min)*nf : (def+c2)*nf);
  ledcWrite(3, c3 < 0 ? (def+duty_min)*nf : (def+c3)*nf);
  ledcWrite(4, c4 < 0 ? (def+duty_min)*nf : (def+c4)*nf);
  
#ifdef output
  bt.print("  ou1:");
  bt.print((def+c1)*nf);
  bt.print("  ou2:");
  bt.print((def+c2)*nf);
  bt.print("  ou3:");
  bt.print((def+c3)*nf);
  bt.print("  ou4:");
  bt.print((def+c4)*nf);

  bt.print("      ch1:");
  bt.print(c1);
  bt.print("  ch2:");
  bt.print(c2);
  bt.print("  ch3:");
  bt.print(c3);
  bt.print("  ch4:");
  bt.print(c4);
  bt.print("     ");
#endif
  return;
}
/*
12bit
75Hz
res:231
off:258
min:260
max:340
*/
void motor::setup()
{
  ledcSetup(1, puls, 12);//1,066,666.666666666666666666666666...まで出来そう?
  ledcSetup(2, puls, 12);//20bit
  ledcSetup(3, puls, 12);
  ledcSetup(4, puls, 12);
  ledcAttachPin(pin.x, 1);
  ledcAttachPin(pin.y, 2);
  ledcAttachPin(pin.x, 3);
  ledcAttachPin(pin.turn, 4);

  ledcWrite(1, duty_reset);
  ledcWrite(2, duty_reset);
  ledcWrite(3, duty_reset);
  ledcWrite(4, duty_reset);
  //delay(500);
  return;
}
