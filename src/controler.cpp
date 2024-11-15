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
    ud = user<int>{1520, 1520, 1110, 1520};
    return;
}
// 読んで返す
user<int> contloler::read()
{
    c.x = static_cast<int>(pulseIn(set.x, HIGH, 20000)) - ud.x;
    c.y = static_cast<int>(pulseIn(set.y, HIGH, 20000)) - ud.y;
    c.z = static_cast<int>(pulseIn(set.z, HIGH, 20000)) - ud.z;
    c.turn = static_cast<int>(pulseIn(set.turn, HIGH, 20000)) - ud.turn;
#ifdef output
    bt.print("   cx:");
    bt.print(c.x);
    bt.print("   cy:");
    bt.print(c.y);
    bt.print("   cz:");
    bt.print(c.z);
    bt.print("   ct:");
    bt.print(c.turn);
    bt.print("     ");
#endif
#ifdef SERIAL_out
    Serial.print("   cx:");
    Serial.print(c.x);
    Serial.print("   cy:");
    Serial.print(c.y);
    Serial.print("   cz:");
    Serial.print(c.z);
    Serial.print("   ct:");
    Serial.print(c.turn);
    Serial.print("     ");
#endif
    return {c.x / 25, c.y / 25, c.z / 7, c.turn / 25};
}
void contloler::reset(user<int> dd)
{
    ud = dd;
}
