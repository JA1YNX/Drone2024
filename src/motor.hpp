#include <Arduino.h>


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