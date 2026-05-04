#ifndef TECLADO_H_
#define TECLADO_H_

#include "stm32f4xx.h"

// Estructura para manejar GPIO de forma genérica
typedef struct {
    GPIO_TypeDef* puerto;
    uint16_t pin;
} RegistroPin_t;

typedef struct {
    RegistroPin_t filas[4];
    RegistroPin_t columnas[4];
} Keypad_Config_t;

// Enumeración de estados según el diagrama de la cátedra
typedef enum {
    STATE_INIT,
    STATE_DETECCION,
    STATE_DEBOUNCE,
    STATE_CONFIRMACION,
    STATE_ESPERO_LIBERACION
} KeypadState_t;

// Prototipos de funciones
void teclado_init(void);
char teclado_fsm_update(void);

#endif
