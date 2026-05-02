
#ifndef TECLADO_H_
#define TECLADO_H_

// Estructura para manejar GPIO de forma genérica
typedef struct {
    // Acá irán punteros a GPIO_TypeDef y los números de Pin para filas y columnas
} Keypad_Config_t;

// Enumeración de estados según el diagrama
typedef enum {
    STATE_INIT,
    STATE_DETECCION,
    STATE_DEBOUNCE,
    STATE_CONFIRMACION,
    STATE_ESPERO_LIBERACION
} KeypadState_t;

// Prototipos de funciones
void teclado_init(void);
char teclado_fsm_update(void); // Retorna la tecla presionada o 0 si no hay nada

#endif
