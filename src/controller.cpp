#include "controller.h"

// data
user<double> controller::data() const
{
    return c;
}
// 今使ってるピン返すやつ
user<int> controller::pin() const
{
    return set;
}
// コンストラクタピン渡す
controller::controller(user<int> set_)
{
    set = set_;
}
// ピン設定
void controller::setup()
{
    // モード設定
    pinMode(set.x, INPUT);
    pinMode(set.y, INPUT);
    pinMode(set.z, INPUT);
    pinMode(set.turn, INPUT);
    ud = UC_DEF;
    return;
}
// 読んで返す
user<double> controller::read()
{
    c.x = static_cast<int>(pulseIn(set.x, HIGH, 20000));
    c.y = static_cast<int>(pulseIn(set.y, HIGH, 20000));
    c.z = static_cast<int>(pulseIn(set.z, HIGH, 20000));
    c.turn = static_cast<int>(pulseIn(set.turn, HIGH, 20000));
    return {(c.x - ud.x) / 90.0, (c.y - ud.y) / 90.0, (c.z - ud.z) * 2.0, (c.turn - ud.turn) / 50.0};
}
void controller::reset(user<double> dd)
{
    ud = dd;
}
