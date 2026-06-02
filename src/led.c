#include "led.h"

static int parpadeosRestantes = 0;
static int tiempoActual = 100;
static LedState_t currentLedState = LED_OUT;
static uint32_t ticksLed = 0;

void led_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_ResetBits(GPIOD, GPIO_Pin_10);
}

void led_start_blink(int parpadeos, int tiempo) {
    extern uint32_t msTicks;

    if (parpadeos > 0) {
        parpadeosRestantes = parpadeos;
        tiempoActual = tiempo;
        currentLedState = LED_ON;

        // forzar que se encienda el primer parpadeo, sino se veia tenue
        GPIO_SetBits(GPIOD, GPIO_Pin_10);
        ticksLed = msTicks; //inicio del conteo sincronizado
    }
}

void led_fsm_update(void) {
    extern uint32_t msTicks;

    switch(currentLedState) {
        case LED_ON:
            if (parpadeosRestantes > 0) {
                GPIO_SetBits(GPIOD, GPIO_Pin_10);
                // Si el tiempo transcurrido es mayor o igual al deseado
                if ((msTicks - ticksLed) >= (uint32_t)tiempoActual) {
                    currentLedState = LED_OFF;
                    ticksLed = msTicks;
                    GPIO_ResetBits(GPIOD, GPIO_Pin_10); // Aseguramos el apagado inmediato
                }
            } else {
                currentLedState = LED_OUT;
            }
            break;

        case LED_OFF:
            GPIO_ResetBits(GPIOD, GPIO_Pin_10);
            if ((msTicks - ticksLed) >= (uint32_t)tiempoActual) {
                parpadeosRestantes--;
                if (parpadeosRestantes > 0) {
                    currentLedState = LED_ON;
                    GPIO_SetBits(GPIOD, GPIO_Pin_10); // Encendido inmediato para el siguiente ciclo
                } else {
                    currentLedState = LED_OUT;
                }
                ticksLed = msTicks;
            }
            break;

        case LED_OUT:
            GPIO_ResetBits(GPIOD, GPIO_Pin_10);
            break;
    }
}
