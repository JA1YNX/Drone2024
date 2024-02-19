#include <Arduino.h>
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