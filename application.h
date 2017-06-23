#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <bcl.h>

typedef enum {
    TL_STOP,
    TL_TIME_SETUP,
    TL_ACTIVE_OFF,
    TL_ACTIVE_ON,
    TL_PAUSE
} tl_status_t;

// LED pulse definition
const int LED_SHORT_PULSE = 100;
const int LED_LONG_PULSE = 1000;

// remote control PINs used to manage camera
const bc_gpio_channel_t TL_PIN_FOCUS = BC_GPIO_P0;
const bc_gpio_channel_t TL_PIN_SHOT = BC_GPIO_P1;

// forward declaration of methods

void init_gpio(bc_gpio_channel_t port);
void start_measuring();
void stop_measuring();
void start_timelapse();
void stop_timelapse();


#endif // _APPLICATION_H
