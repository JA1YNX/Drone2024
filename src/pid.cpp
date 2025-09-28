#include "pid.h"

namespace PID_F
{
    double PID::calc(double e, double p, double i, double d, double deriv_meas)
    {
        // 積分
        integrator += 0.5 * (e + e_prev) * T;
        if (integrator > integrator_max)
            integrator = integrator_max;
        if (integrator < integrator_min)
            integrator = integrator_min;

        // PID出力
        double u = p * e + i * integrator - d * deriv_meas;

        e_prev = e;
        return u;
    }
    double AngleController::calc(double y, double r, double rate,
                                 double p_angle, double i_angle, double d_angle,
                                 double p_rate, double i_rate, double d_rate)
    {
        // ---- 外側ループ：角度PID ----
        double e_angle = r - y;
        // D項は角速度を利用
        double rate_ref = anglePID.calc(e_angle, p_angle, i_angle, d_angle, rate);

        // ---- 内側ループ：角速度PID ----
        double e_rate = rate_ref - rate;
        // D項は角加速度が理想だが、ジャイロからは直接取れないので
        // ここは rate の変化を近似的に使う or LPF で滑らかにする
        double u = ratePID.calc(e_rate, p_rate, i_rate, d_rate, 0.0);

        return u;
    }
}
