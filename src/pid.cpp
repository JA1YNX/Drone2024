#include "pid.h"

namespace PID_F
{
    /*double Pid::calc(double y, double r, double a, double p, double i, double d)
    {
        y -= abe;         // バイアス補正
        double e = r - y; // 誤差

        // 積分（台形則）
        ie += (e)*0.01; // note:制御周期0.01sで固定

        // 微分（角加速度に基づく）
        double de = -a; // note:角加速度が正ならば角速度が増加しているので、誤差は減少する方向に働く

        // 出力
        double u = p * e + i * ie + d * de;

        return u;
    }
    double Pid::calc(double y, double r, double p, double i, double d)
    {
        //
        // double ret = 0;
        // double e = r - y;
        // ret += e * KP;          // P項
        // ret += (e - pree) * KD; // D項
        // ie += (e + pree) / 2;
        // ret += ie * KI; // I項

        // 下記変数は既に与えられているものとする
        // y       : 現在の出力
        // r       : 現在の目標値
        // e_pre   : 前回の誤差
        // T       : 制御周期
        // KP,KI,KD: P,I,Dゲイン

        // y -= abe;                      // 絶対誤差を引く
        static double e_pre = 0; // 前回の誤差
        // static double T = 0.01;  // 制御周期（秒）//todo:制御周期セット
        static double T_pre = 1; // 制御周期（秒）//todo:制御周期セット
        double T_now = 2;
        double T = T_now - T_pre;
        T_pre = T_now;
        T = 1;
        // static double ie = 0;          // 誤差の積分値
        // double de = 0;                 // 誤差の微分値
        // double e;                      // 誤差
        // double u;                      // 出力値
        // e = r - y;                     // 誤差を計算
        // de = (e - e_pre) / T;          // 誤差の微分を近似計算
        // ie = ie + (e + e_pre) * T / 2; // 誤差の積分を近似計算
        // u = p * e + i * ie + d * de;   // PID制御の式にそれぞれを代入
        // return u;

        y -= abe;         // バイアス補正
        double e = r - y; // 誤差

        // 積分（台形則）
        ie += (e + e_pre) * T / 2.0;

        // 微分（誤差に基づく）
        double de = (e - e_pre) / T;

        // 出力
        double u = p * e + i * ie + d * de;

        // 前回誤差更新
        e_pre = e;

        return u;
    }
    Pid::Pid(double _abe)
    {
        abe = _abe;
    }
    Pid::~Pid()
    {
    }
    void Pid::reset(double _abe)
    {
        ie = 0;
        abe = _abe;
    }*/
}
