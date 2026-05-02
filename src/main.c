
/*
 * Trabajo Práctico N°2: GPIO & Teclado
 * * Asignatura: Técnicas Digitales II
 * Alumno: Agustín González
 * Institución: UTN - Facultad Regional Bahía Blanca
 * * Descripción:
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

int main(void) {
    // 1. Inicialización de periféricos y teclado_init()

    while (1) {
        // Actualizar FSM del teclado y obtener tecla
        char tecla = teclado_fsm_update();

        // Si la tecla es una letra (A-D), cambiar tiempoBase

        // C. Si la tecla es número, calcular parpadeosRestantes = tecla + 1

        // D. FSM del LED
        switch(ledState) {
            case LED_ON:
                // Encender LED, esperar tiempoBase usar for()
                // Pasar a OFF
                break;
            case LED_OFF:
                // Apagar LED, esperar tiempoBase
                // Decrementar parpadeosRestantes
                // Si parpadeos > 0 volver a ON, sino a OUT
                break;
            // ...
        }
    }
}
