#include "controller.h"

// data
user<int> controller::data() const
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
user<int> controller::read()
{
    c.x = static_cast<int>(pulseIn(set.x, HIGH, 20000));
    c.y = static_cast<int>(pulseIn(set.y, HIGH, 20000));
    c.z = static_cast<int>(pulseIn(set.z, HIGH, 20000));
    c.turn = static_cast<int>(pulseIn(set.turn, HIGH, 20000));
    return {(c.x - ud.x) / 90, (c.y - ud.y) / 90, (c.z - ud.z) * 2, (c.turn - ud.turn) / 50};
}
void controller::reset(user<int> dd)
{
    ud = dd;
}
