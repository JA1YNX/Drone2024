#include "led.h"

int set_led(uled_status status_, int y_va)
{
    switch (status_)
    {
    case uled_status::ready:
        digitalWrite(R_pin, ((int)uled_status::ready & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::ready & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::ready & ledbin::Y) * y_va);
        break;

    case uled_status::set:
        digitalWrite(R_pin, ((int)uled_status::set & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::set & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::set & ledbin::Y) * y_va);
        break;

    case uled_status::wait:
        digitalWrite(R_pin, ((int)uled_status::wait & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::wait & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::wait & ledbin::Y) * 255);
        break;

    case uled_status::stop:
        digitalWrite(R_pin, ((int)uled_status::stop & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::stop & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::stop & ledbin::Y) * y_va);
        break;
    case uled_status::none:
        digitalWrite(R_pin, ((int)uled_status::none & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::none & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::none & ledbin::Y) * 0);
        break;
    default:
        digitalWrite(R_pin, ((int)uled_status::def & ledbin::R));
        digitalWrite(G_pin, ((int)uled_status::def & ledbin::G));
        ledcWrite(Y_pin, ((int)uled_status::def & ledbin::Y) * y_va);
        break;
    }
    return 0;
}
int setup_led()
{
    // R
    pinMode(R_pin, OUTPUT);
    digitalWrite(R_pin, HIGH);
    // Y
    ledcSetup(Y_pin, puls, 8);
    ledcAttachPin(Y_pin, Y_pin);
    ledcWrite(Y_pin, 255);
    // other
    pinMode(PIN_ch5, INPUT);
    pinMode(G_pin, OUTPUT);
    set_led(uled_status::none);
    return 0;
}