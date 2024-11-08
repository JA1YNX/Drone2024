#include "pid.h"

namespace PID_F
{
    /*
    // 正直これはわかんない
    double pid_controller(double setpoint, double current_value, double &previous_error)
    {
        double error = setpoint - current_value;
        double integral = 0.0;   // 積分値
        double derivative = 0.0; // 微分値

        integral += error;                                            // 積分値の更新
        derivative = error - previous_error;                          // 微分値の更新
        double output = KP * error + KI * integral + KD * derivative; // 出力計算
        previous_error = error;                                       // 現在の誤差を保存
        return output;
    }
    
    // 上の関数を自分で書き換えてみたけどわかんなかった
    /// @brief PID
    /// @param now now_val今の角度
    /// @param pre pre_err前のerr
    /// @param set setpoint目標値
    /// @param integ integral前の値
    /// @param out 出力
    /// @return now_err今のerr
    double pid_(double now, double pre, double set, double &integ, double &out)
    {
        double err = pre;
        double p = KP * (set - now);
        double err1 = now - set;
        integ += (now - set + err) / 2.0 * DELTA_T;
        double i = KI * integ;
        double d = KD * (err1 - err) / DELTA_T;
        out = (p + i + d);
        return err;
    }
    */
    /*

    m.c1=pid(,,);

    */

    /// @brief PID計算
    /// @param y 入力（今）
    /// @param r 目標
    /// @return 出力
    double Pid::culc(double y, double r)
    {
        double ret = 0;
        double e = r - y;
        ret += e * KP;          // P項
        ret += (e - pree) * KD; // D項
        ie += (e + pree) / 2;
        ret += ie * KI; // I項
        return ret;
    }
    /// @brief コンストラクタ
    /// @param pre 初期化時点の誤差
    Pid::Pid(double pre)
    {
        pree = pre;
    }
    Pid::~Pid()
    {
    }
    /*
    pid::~pid()
    {
    }

    double pid::pid_culc(double feedback_val, double target_val)
    {

        double p, i, d;

        error[0] = error[1];
        error[1] = feedback_val - target_val;
        integral += (error[1] + error[0]) / 2.0 * DELTA_T;

        p = KP * error[1];
        i = KI * integral;
        d = KD * (error[1] - error[0]) / DELTA_T;

        return (p + i + d);
    }
    */
}
