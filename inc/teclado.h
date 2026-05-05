#ifndef TECLADO_H_
#define TECLADO_H_

#include "stm32f4xx.h"


typedef struct {
    GPIO_TypeDef* puerto;
    uint16_t pin;
} RegistroPin_t;

typedef struct {
    RegistroPin_t filas[4];
    RegistroPin_t columnas[4];
} Keypad_Config_t;

// enumeracion de estados
typedef enum {
    STATE_INIT,
    STATE_DETECCION,
    STATE_DEBOUNCE,
    STATE_CONFIRMACION,
    STATE_ESPERO_LIBERACION
} KeypadState_t;


void teclado_init(void);
char teclado_fsm_update(void);

#endif
