#include "led.h"

int set_led(led_status status_, int y_va)
{
    switch (status_)
    {
    case led_status::ready:
        digitalWrite(R_pin, ((int)led_status::ready & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::ready & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::ready & ledbin::Y) * y_va);
        break;

    case led_status::set:
        digitalWrite(R_pin, ((int)led_status::set & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::set & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::set & ledbin::Y) * y_va);
        break;

    case led_status::wait:
        digitalWrite(R_pin, ((int)led_status::wait & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::wait & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::wait & ledbin::Y) * 255);
        break;

    case led_status::stop:
        digitalWrite(R_pin, ((int)led_status::stop & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::stop & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::stop & ledbin::Y) * y_va);
        break;
    case led_status::none:
        digitalWrite(R_pin, ((int)led_status::none & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::none & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::none & ledbin::Y) * 0);
        break;
    default:
        digitalWrite(R_pin, ((int)led_status::def & ledbin::R));
        digitalWrite(G_pin, ((int)led_status::def & ledbin::G));
        ledcWrite(Y_pin, ((int)led_status::def & ledbin::Y) * y_va);
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
    set_led(led_status::none);
    return 0;
}