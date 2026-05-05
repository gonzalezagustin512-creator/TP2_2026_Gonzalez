/*
 * Trabajo Práctico N°2: GPIO & Teclado
 * Asignatura: Técnicas Digitales II
 * Alumno: Agustín González
 * Institución: UTN - Facultad Regional Bahía Blanca
 */
#include "stm32f4xx.h"
#include "teclado.h"

typedef enum {
    LED_OUT,
    LED_ON,
    LED_OFF
} LedState_t;

int parpadeosRestantes = 0;
int tiempoBase = 100; // Velocidad inicial por defecto 100ms
LedState_t ledState = LED_OUT;

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 10000); i++) {
        __NOP();
    }
}

int main(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    teclado_init();

    // comienzo sin parpadeos iniciales
    parpadeosRestantes = 0;
    ledState = LED_OUT;

    while (1) {
        // actualizar FSM del teclado y obtener tecla
        char tecla = teclado_fsm_update();

        if (tecla != 0) {
            // si la tecla es una letra (A-D), cambiar tiempoBase
            //  A=50ms, B=90ms, C=110ms, D=220ms
            switch(tecla) {
                case 'A': tiempoBase = 50;  break;
                case 'B': tiempoBase = 90;  break;
                case 'C': tiempoBase = 110; break;
                case 'D': tiempoBase = 220; break;
            }

            // asignacion de parpadeos
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
                default:  parpadeosNuevos = 0; break; // ignora letra o simbolo
            }

            // si es un numero valido prende led
            if (parpadeosNuevos != 0) {
                parpadeosRestantes = parpadeosNuevos;
                ledState = LED_ON;
            }
        }

        // FSM del LED
        switch(ledState) {
            case LED_ON:
                if (parpadeosRestantes > 0) {
                    GPIO_SetBits(GPIOD, GPIO_Pin_10);
                    delay_ms(tiempoBase);
                    ledState = LED_OFF;
                } else {
                    ledState = LED_OUT;
                }
                break;

            case LED_OFF:
                GPIO_ResetBits(GPIOD, GPIO_Pin_10);
                delay_ms(tiempoBase);
                parpadeosRestantes--;
                ledState = LED_ON;
                break;

            case LED_OUT:
                GPIO_ResetBits(GPIOD, GPIO_Pin_10);
                break;
        }
    }
}
