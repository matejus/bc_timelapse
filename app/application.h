#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <bcl.h>

// remote control PINs used to manage camera
#define TL_PIN_FOCUS BC_GPIO_P0
#define TL_PIN_SHOT BC_GPIO_P1


// LED pulse definition
#define LED_SHORT_PULSE 100
#define LED_LONG_PULSE 1000

// mode how the module will be used
typedef enum {
    TL_TIMELAPSE = 100,
    TL_STARTRAIL = 1000
} tl_mode;

// state of the application during the run
typedef enum {
    TL_STOP,
    TL_TIME_SETUP,
    TL_ACTIVE_OFF,
    TL_ACTIVE_ON,
    TL_PAUSE
} tl_status_t;




// forward declaration of methods
void init_gpio(bc_gpio_channel_t port);
void start_measuring();
void stop_measuring();
void start_timelapse();
void stop_timelapse();


#endif // _APPLICATION_H
