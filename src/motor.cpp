#include "motor.h"

void motor::stop()
{
    nf = 0;
    rotate();
}
void motor::rotate() const
{
    ledcWrite(1, (def + c1) * nf + duty_low);
    ledcWrite(2, (def + c2) * nf + duty_low);
    ledcWrite(3, (def + c3) * nf + duty_low);
    ledcWrite(4, (def + c4) * nf + duty_low);
    return;
}
/*
12bit
75Hz
res:231
off:258
min:260
max:340
*/
void motor::setup() const
{
    ledcSetup(1, puls, 12); // 1,066,666.666666666666666666666666...まで出来そう?
    ledcSetup(2, puls, 12); // 20bit
    ledcSetup(3, puls, 12);
    ledcSetup(4, puls, 12);
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
