#include <application.h>

// Button instance
bc_button_t _button;
// LED instance
bc_led_t _led;

// current application status
tl_status_t _tl_status = TL_STOP;

// to measure required time
bc_tick_t _timer;
int _duration;
bc_scheduler_task_id_t _task_id;

// configure or manage the timelapse manager
void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;
    
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        switch (_tl_status){
            case TL_STOP:
                start_measuring();
                break;
            case TL_TIME_SETUP:
                stop_measuring();
                start_timelapse();
                break;
            case TL_ACTIVE_OFF:
            case TL_ACTIVE_ON:
                stop_timelapse();
                break;
            case TL_PAUSE:
                start_timelapse();
                break;

            default:
                bc_led_set_mode(&_led, BC_LED_MODE_BLINK_SLOW);
                return;        
        }
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        // reset to STOP mode
        bc_led_set_mode(&_led, BC_LED_MODE_OFF);
        bc_led_pulse(&_led, LED_LONG_PULSE);
        _tl_status = TL_STOP;
    }
}

// initialize all components
void application_init(void)
{
    // Initialize LED
    bc_led_init(&_led, BC_GPIO_LED, false, false);
    bc_led_pulse(&_led, 2000);

    // Initialize button - to set radio communication
    bc_button_init(&_button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&_button, button_event_handler, NULL);
    
    init_gpio(TL_PIN_SHOT);
    init_gpio(TL_PIN_FOCUS);
    
}


void init_gpio(bc_gpio_channel_t port)
{
    bc_gpio_init(port);
    bc_gpio_set_mode(port, BC_GPIO_MODE_OUTPUT);
    bc_gpio_set_output(port, 1);
}

void start_measuring()
{
    bc_led_set_mode(&_led, BC_LED_MODE_BLINK);
    _timer = bc_tick_get();
    _tl_status = TL_TIME_SETUP;
}

void stop_measuring()
{
    _duration = bc_tick_get() - _timer;
    bc_led_set_mode(&_led, BC_LED_MODE_OFF);
    bc_led_pulse(&_led, LED_LONG_PULSE);
    _tl_status = TL_PAUSE;
}

void camera_focus(bool active)
{
    bc_gpio_set_output(TL_PIN_FOCUS, active?0:1);
}

void camera_shot(void *param)
{
    (void) param;

    if (_tl_status==TL_ACTIVE_OFF)
    {
        bc_gpio_set_output(TL_PIN_SHOT, 0);
        _tl_status = TL_ACTIVE_ON;
        bc_scheduler_plan_relative(_task_id, 100);
        bc_led_set_mode(&_led, BC_LED_MODE_ON);
    }
    else
    {
        bc_gpio_set_output(TL_PIN_SHOT, 1);
        _tl_status = TL_ACTIVE_OFF;
        bc_scheduler_plan_relative(_task_id, _duration);
        bc_led_set_mode(&_led, BC_LED_MODE_OFF);
    }
}

void start_timelapse()
{
    bc_led_pulse(&_led, LED_LONG_PULSE);
    camera_focus(true);
    _task_id = bc_scheduler_register(camera_shot, NULL, bc_tick_get());

    _tl_status = TL_ACTIVE_OFF;
    bc_gpio_set_output(TL_PIN_SHOT, 1);
    bc_scheduler_plan_now(_task_id);
}

void stop_timelapse()
{
    bc_scheduler_unregister(_task_id);
    camera_focus(false);
    _tl_status = TL_PAUSE;
    bc_led_pulse(&_led, LED_LONG_PULSE);
}