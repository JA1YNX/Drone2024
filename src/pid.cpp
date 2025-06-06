#include "pid.h"

namespace PID_F
{
    double Pid::calc(double y, double r)
    {
        /*
        double ret = 0;
        double e = r - y;
        ret += e * KP;          // P項
        ret += (e - pree) * KD; // D項
        ie += (e + pree) / 2;
        ret += ie * KI; // I項
        */
        // 下記変数は既に与えられているものとする
        // y       : 現在の出力
        // r       : 現在の目標値
        // e_pre   : 前回の誤差
        // T       : 制御周期
        // KP,KI,KD: P,I,Dゲイン
        y -= abe;                       // 絶対誤差を引く
        static double e_pre = 0;        // 前回の誤差
        static double T = 0.01;         // 制御周期（秒）
        double ie = 0;                  // 誤差の積分値
        double de = 0;                  // 誤差の微分値
        double e;                       // 誤差
        double u;                       // 出力値
        e = r - y;                      // 誤差を計算
        de = (e - e_pre) / T;           // 誤差の微分を近似計算
        ie = ie + (e + e_pre) * T / 2;  // 誤差の積分を近似計算
        u = KP * e + KI * ie + KD * de; // PID制御の式にそれぞれを代入
        return u;
    }
    Pid::Pid(double p, double i, double d, double _abe)
    {
        KP = p;
        KI = i;
        KD = d;
        abe = _abe;
    }
    Pid::~Pid()
    {
    }
    void Pid::reset(double _abe)
    {
        ie = 0;
        abe = _abe;
    }
}
