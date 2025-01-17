#include "controler.h"

// data
user<int> contloler::data() const
{
    return c;
}
// 今使ってるピン返すやつ
user<int> contloler::pin() const
{
    return set;
}
// コンストラクタピン渡す
contloler::contloler(user<int> set_)
{
    set = set_;
}
// ピン設定
void contloler::setup()
{
    // モード設定
    pinMode(set.x, INPUT);
    pinMode(set.y, INPUT);
    pinMode(set.z, INPUT);
    pinMode(set.turn, INPUT);
    // delay(5000);
    ud = UC_DEF;
    return;
}
// 読んで返す
user<int> contloler::read()
{
    c.x = static_cast<int>(pulseIn(set.x, HIGH, 20000));
    c.y = static_cast<int>(pulseIn(set.y, HIGH, 20000));
    c.z = static_cast<int>(pulseIn(set.z, HIGH, 20000));
    c.turn = static_cast<int>(pulseIn(set.turn, HIGH, 20000));
#ifdef SERIAL_out
    /*
    Serial.print(" cx:");
    Serial.print(c.x);
    Serial.print(" y:");
    Serial.print(c.y);
    Serial.print(" z:");
    Serial.print(c.z);
    Serial.print(" t:");
    Serial.print(c.turn);
    Serial.print(" ");
    */
#endif
    return {(c.x - ud.x) / 25, (c.y - ud.y) / 25, (c.z - ud.z) / 7, (c.turn - ud.turn) / 25};
}
void contloler::reset(user<int> dd)
{
    ud = dd;
}
