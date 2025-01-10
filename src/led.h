#ifndef LED
#define LED
#include "conf.h"
enum uled_status
{
    ready = 0,
    set = 1,
    wait = 2,
    stop = 3,
    none = 4,
};
int set_led(uled_status status_, int y_va = 0);
int setup_led();
#endif