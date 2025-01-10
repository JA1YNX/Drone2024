#include "led.h"

int set_led(uled_status status_, int y_va)
{
    switch (status_)
    {
    case uled_status::ready:
        digitalWrite(R_pin, LOW);
        digitalWrite(G_pin, HIGH);
        ledcWrite(Y_pin, y_va);
        break;

    case uled_status::set:
        digitalWrite(R_pin, HIGH);
        digitalWrite(G_pin, LOW);
        ledcWrite(Y_pin, y_va);
        break;

    case uled_status::wait:
        digitalWrite(R_pin, LOW);
        digitalWrite(G_pin, LOW);
        ledcWrite(Y_pin, 255);
        break;

    case uled_status::stop:
        digitalWrite(R_pin, HIGH);
        digitalWrite(G_pin, HIGH);
        ledcWrite(Y_pin, y_va);
        break;
    case uled_status::none:
        digitalWrite(R_pin, LOW);
        digitalWrite(G_pin, LOW);
        ledcWrite(Y_pin, y_va);
        break;
    default:
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