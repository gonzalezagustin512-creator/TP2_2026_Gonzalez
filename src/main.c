/*
 * Trabajo Práctico N°2: GPIO & Teclado
 * Asignatura: Técnicas Digitales II
 * Alumno: Agustín González
 * Institución: UTN - Facultad Regional Bahía Blanca
 *
 * Descripción:
 * Implementación de una máquina de estados para el escaneo de un teclado
 * matricial de 4x4 y control de secuencias de un LED mediante intermitencias
 * y bases de tiempo variables.
 */

#include "stm32f4xx.h"
#include "teclado.h"

typedef enum {
    LED_OUT,
    LED_ON,
    LED_OFF
} LedState_t;

int parpadeosRestantes = 0;
int tiempoBase = 100;
LedState_t ledState = LED_OUT;

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 21000); i++) {
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

    parpadeosRestantes = 5;
    ledState = LED_ON;

    while (1) {
        // A. Actualizar FSM del teclado y obtener tecla
        char tecla = teclado_fsm_update();


        //        SECCION TEST TECLADO

        if (tecla != 0) {
            // Si es una letra (A, B, C o D)
            if (tecla >= 'A' && tecla <= 'D') {
                GPIO_SetBits(GPIOD, GPIO_Pin_10); // Prender fijo para indicar letra
                delay_ms(500);
                GPIO_ResetBits(GPIOD, GPIO_Pin_10);
            }
            // Si es un número (0-9) O un símbolo (*, #)
            if ((tecla >= '0' && tecla <= '9') || tecla == '*' || tecla == '#') {
                // Hacemos 2 destellos muy rápidos para diferenciar de las letras
                for(int t=0; t<2; t++) {
                    GPIO_SetBits(GPIOD, GPIO_Pin_10);
                    delay_ms(50);
                    GPIO_ResetBits(GPIOD, GPIO_Pin_10);
                    delay_ms(50);
                }
            }
        }
        // ==========================================

        // B. Si la tecla es una letra (A-D), cambiar tiempoBase
        // (Sigue como comentario...)

        // C. Si la tecla es número, calcular parpadeosRestantes = tecla + 1
        // (Sigue como comentario...)

        // D. FSM del LED
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
