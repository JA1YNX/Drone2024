#pragma once

#include "conf.h"

/*
正直PID制御についてあんまりわかってないけどいろんなサイト見てみてこんな感じかな？って感じで実装した結果
*/
namespace PID_F
{
    class Pid
    {
    private:
        double pree = 0;
        double ie = 0;
        double KP = 0;
        double KI = 0;
        double KD = 0;

    public:
        /// @brief コンストラクタ
        /// @param 初期化時点の誤差
        Pid(double p, double i, double d, double pre = 0);
        ~Pid();
        /// @brief PID計算
        /// @param y 現在値
        /// @param r 目標値
        /// @return 出力
        double calc(double y, double r);
        /// @brief リセット
        /// @param 初期化誤差
        void reset(double pre = 0);
    };

}
