#include "motor.h"
// #define cutsighn(x) ((x)<0?0:(x))
static inline unsigned int cutsighn(int x)
{
    return x < 0 ? 0 : x;
}
void motor::stop()
{
    nf = 0;
    rotate();
}
void motor::rotate() const
{
    ledcWrite(1, cutsighn(def + c1) * nf + duty_low);
    ledcWrite(2, cutsighn(def + c2) * nf + duty_low);
    ledcWrite(3, cutsighn(def + c3) * nf + duty_low);
    ledcWrite(4, cutsighn(def + c4) * nf + duty_low);
    return;
}

void motor::setup() const
{
    ledcSetup(1, puls, bits); // 1,066,666.666666666666666666666666...まで出来そう?
    ledcSetup(2, puls, bits); // 20bit
    ledcSetup(3, puls, bits);
    ledcSetup(4, puls, bits);
    ledcAttachPin(pin.x, 1);
    ledcAttachPin(pin.y, 2);
    ledcAttachPin(pin.z, 3);
    ledcAttachPin(pin.turn, 4);

    ledcWrite(1, duty_min - 1);
    ledcWrite(2, duty_min - 1);
    ledcWrite(3, duty_min - 1);
    ledcWrite(4, duty_min - 1);
    return;
}
