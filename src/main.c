/*
 * Trabajo Práctico N°2: GPIO & Teclado
 * Asignatura: Técnicas Digitales II
 * Alumno: Agustín González
 * Institución: UTN - Facultad Regional Bahía Blanca
 */
#include "stm32f4xx.h"
#include "teclado.h"
#include "led.h"

//variable cuenta los ms
volatile uint32_t msTicks = 0;

// temporizador integrado ejecuta cada 1ms
void SysTick_Handler(void) {
    msTicks++;
}

/* //lo llevo a led.h
typedef enum {
    LED_OUT,
    LED_ON,
    LED_OFF
} LedState_t;
*/

int parpadeosRestantes = 0;
int tiempoBase = 100;
// LedState_t ledState = LED_OUT; // lo llevo a led.h

/* // forma anterior de delay
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 10000); i++) {
        __NOP();
    }
}
*/

int main(void) {
    // Configuracion de SysTick para disparar cada 1ms
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);
    }

    led_init(); // funcion de inicializacion para el led
    teclado_init();

    parpadeosRestantes = 0;

    while (1) {
        // Actualizar FSM del teclado
        char tecla = teclado_fsm_update();

        if (tecla != 0) {
            switch(tecla) {
                case 'A': tiempoBase = 50;  break;
                case 'B': tiempoBase = 90;  break;
                case 'C': tiempoBase = 110; break;
                case 'D': tiempoBase = 220; break;
            }

            int parpadeosNuevos = 0;
            switch(tecla) {
                case '0': parpadeosNuevos = 1;  break;
                case '1': parpadeosNuevos = 2;  break;
                case '2': parpadeosNuevos = 3;  break;
                case '3': parpadeosNuevos = 4;  break;
                case '4': parpadeosNuevos = 5;  break;
                case '5': parpadeosNuevos = 6;  break;
                case '6': parpadeosNuevos = 7;  break;
                case '7': parpadeosNuevos = 8;  break;
                case '8': parpadeosNuevos = 9;  break;
                case '9': parpadeosNuevos = 10; break;
                default:  parpadeosNuevos = 0; break;
            }
            if (parpadeosNuevos != 0) {
                led_start_blink(parpadeosNuevos, tiempoBase);
            }
        }
        // fsm led (Ahora en led.c y no bloqueante)
        led_fsm_update();

    }
}
