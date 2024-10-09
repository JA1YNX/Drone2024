#include "conf.h"

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
