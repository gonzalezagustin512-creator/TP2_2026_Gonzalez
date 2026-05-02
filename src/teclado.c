#include "stm32f4xx.h"
#include "teclado.h"

// Variables privadas para el estado actual
static KeypadState_t currentState = STATE_INIT;

void teclado_init(void) {
    // Configuración de pines de filas (salidas) y columnas (entradas con pull-up/down)
    // Pasar a STATE_DETECCION una vez terminado
}

char teclado_fsm_update(void) {
    char teclaDetectada = 0;

    switch(currentState) {
        case STATE_DETECCION:
            // Acción: Escanear columnas en bucle
            // Si detecta cambio -> ir a STATE_DEBOUNCE
            break;

        case STATE_DEBOUNCE:
            // Acción: Esperar 20ms
            // Al terminar tiempo -> ir a STATE_CONFIRMACION
            break;

        case STATE_CONFIRMACION:
            // Acción: Leer filas y columnas de nuevo
            // Si se mantiene -> ir a STATE_ESPERO_LIBERACION y guardar tecla
            // Si no -> Volver a STATE_DETECCION (Falsa alarma)
            break;

        case STATE_ESPERO_LIBERACION:
            // Acción: Esperar a que la tecla deje de estar presionada
            // Al liberar -> Volver a STATE_DETECCION
            break;

        default:
            currentState = STATE_DETECCION;
            break;
    }

    return teclaDetectada;
}
