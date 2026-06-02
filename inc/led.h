#ifndef LED_H_
#define LED_H_

#include "stm32f4xx.h"

// estados fsm led
typedef enum {
    LED_OUT,
    LED_ON,
    LED_OFF
} LedState_t;

//funciones

void led_init(void);
void led_start_blink(int parpadeos, int tiempoBase);
void led_fsm_update(void);

#endif
