#ifndef PID
#define PID
#include "conf.h"
/*
正直PID制御についてあんまりわかってないけどいろんなサイト見てみてこんな感じかな？って感じで実装した結果
*/
namespace PID_F
{
#define DELTA_T 1
    //double pid_controller(double, double, double &);
    //double pid_(double, double, double, double &, double &);

    class Pid
    {
    private:
        double pree = 0;
        double ie = 0;

    public:
        Pid(double);
        ~Pid();
        double culc(double, double);
    };

    /*
    今のところこのクラスで制御することになりそう
    変数については参考にしたサイトに載っていたものをクラスに書き換えただけなので正直わかんない
    モーター毎か角度毎にインスタンス化しといて制御する感じだと思う
    */
    /*
    class pid
    {
        // https://emb.macnica.co.jp/articles/15859/
    private:
        double error[2] = {0, 0};
        double integral = 0;

    public:
        double pid_culc(double, double);
        pid();
        ~pid();
    };
    */
}
#endif
