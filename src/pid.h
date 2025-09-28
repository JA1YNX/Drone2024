#pragma once

#include "conf.h"

namespace PID_F
{
    class PID
    {
    public:
        double e_prev = 0.0;
        double integrator = 0.0;
        double integrator_min = -100.0, integrator_max = 100.0;
        double T = 0.01; // 制御周期[s]

        /// @brief PID計算
        /// @param e 現在の誤差
        /// @param p Pゲイン
        /// @param i Iゲイン
        /// @param d Dゲイン
        /// @param deriv_meas 微分測定値（角加速度など）
        /// @return 出力
        double calc(double e, double p, double i, double d, double deriv_meas);
    };
    class AngleController
    {
    public:
        PID anglePID; // 角度用PID
        PID ratePID;  // 角速度用PID

        double T = 0.01; // 制御周期

        /// @param y 現在角度
        /// @param r 目標角度
        /// @param rate 現在角速度 (ジャイロ)
        /// @param p_angle, i_angle, d_angle 角度ループPIDゲイン
        /// @param p_rate,  i_rate,  d_rate  角速度ループPIDゲイン
        double calc(double y, double r, double rate,
                    double p_angle, double i_angle, double d_angle,
                    double p_rate, double i_rate, double d_rate);
    };
}
