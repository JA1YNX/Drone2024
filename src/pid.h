#ifndef PID
#define PID
#include "conf.h"
namespace PID_F
{
#define DELTA_T 1

    double pid_controller(double setpoint, double current_value, double& previous_error)
    {
        double error = setpoint - current_value;
        double integral = 0.0; // 積分値
        double derivative = 0.0; // 微分値

        integral += error; // 積分値の更新
        derivative = error - previous_error; // 微分値の更新
        double output = Kp * error + Ki * integral + Kd * derivative; // 出力計算
        previous_error = error; // 現在の誤差を保存
        return output;
    }


    /// @brief PID
    /// @param now now_val今の角度
    /// @param pre pre_err前のerr
    /// @param set setpoint目標値
    /// @param integ integral前の値
    /// @param out 出力
    /// @return now_err今のerr
    double pid(double now,double pre, double set,double& integ,double& out)
    {
        double err = pre;
        double p = Kp*(set-now);
        double err1 = now - set;
        integ += (now - set + err) / 2.0 * DELTA_T;
        double i = Ki*integ;
        double d = Kd*(err1 - err)/DELTA_T;
        out = (p + i + d);
        return err;
    }
    /*

    m.c1=pid(,,);

    */

    class pid
    {
        // https://emb.macnica.co.jp/articles/15859/
    private:
        double error[2] = {0,0};
        double integral = 0;
    public:
        double pid_culc(double, double);
        pid();
        ~pid();
    };

    pid::pid()
    {
    }

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
        d = KD * (error[1] - error[0])/DELTA_T;

    return (p + i + d);

    }
}
#endif
