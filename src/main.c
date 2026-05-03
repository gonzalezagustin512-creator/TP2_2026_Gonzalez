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

// Estados para la FSM del LED
typedef enum {
    LED_OUT,
    LED_ON,
    LED_OFF
} LedState_t;

// Variables globales para tiempos y parpadeos
int parpadeosRestantes = 0;
int tiempoBase = 100;
LedState_t ledState = LED_OUT; // Estado inicial

// Función de delay
void delay_ms(uint32_t ms) {
    // Para 168MHz, 21000 iteraciones aproximan 1ms
    for (uint32_t i = 0; i < (ms * 21000); i++) {
        __NOP(); // No Operation: solo para que el compilador no borre el for
    }
}

int main(void) {
    // Estructura de configuración para GPIO
    GPIO_InitTypeDef GPIO_InitStruct;

    // Habilitar el reloj
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Configurar el Pin PD10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;    // Salida
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   // Push-Pull
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

    // Aplicar configuración al puerto D
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // Inicialización del Teclado (Llamada a teclado.c)
    teclado_init();

    // TEST
    // 5 parpadeos para validar que el delay y el LED funcionan al arrancar
    parpadeosRestantes = 5;
    ledState = LED_ON;

    while (1) {
        // A. Actualizar FSM del teclado y obtener tecla
        char tecla = teclado_fsm_update();

        // B. Si la tecla es una letra (A-D), cambiar tiempoBase
        // (Aquí agregarás los if/switch para las letras más adelante)

        // C. Si la tecla es número, calcular parpadeosRestantes = tecla + 1
        // (Aquí agregarás la conversión de char a int más adelante)

        // D. FSM del LED
        switch(ledState) {
            case LED_ON:
                if (parpadeosRestantes > 0) {
                    GPIO_SetBits(GPIOD, GPIO_Pin_10); // Encender LED
                    delay_ms(tiempoBase);             // Esperar (for)
                    ledState = LED_OFF;               // Siguiente estado
                } else {
                    ledState = LED_OUT;               // Terminar secuencia
                }
                break;

            case LED_OFF:
                GPIO_ResetBits(GPIOD, GPIO_Pin_10);   // Apagar LED
                delay_ms(tiempoBase);                 // Esperar (for)
                parpadeosRestantes--;                 // Descontar parpadeo
                ledState = LED_ON;                    // Repetir ciclo
                break;

            case LED_OUT:
                GPIO_ResetBits(GPIOD, GPIO_Pin_10);   // Asegurar que quede apagado
                // En este estado el LED no hace nada hasta que cargues parpadeosRestantes > 0
                break;
        }
    }
}
