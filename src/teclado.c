#include "teclado.h"
#include "stm32f4xx_rcc.h"

static KeypadState_t currentState = STATE_INIT;
static char teclaGuardada = 0;
static uint32_t contadorTicks = 0;

//pines de filas y columnas
static Keypad_Config_t miTeclado = {
    .filas = {
        {GPIOE, GPIO_Pin_8},  // fila 1
        {GPIOE, GPIO_Pin_9},  // fila 2
        {GPIOE, GPIO_Pin_10}, // fila 3
        {GPIOE, GPIO_Pin_11}  // fila 4
    },
    .columnas = {
        {GPIOC, GPIO_Pin_0},  // columna 1
        {GPIOC, GPIO_Pin_3},  // columna 2
        {GPIOC, GPIO_Pin_2},  // columna 3
        {GPIOA, GPIO_Pin_0}   // columna 4
    }
};

static char mapaTeclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void teclado_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    // Habilito relojes de los puertos
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);

    // ponemos las filas como output
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    for (int i = 0; i < 4; i++) {
        GPIO_InitStruct.GPIO_Pin = miTeclado.filas[i].pin;
        GPIO_Init(miTeclado.filas[i].puerto, &GPIO_InitStruct);
    }

    // configuracion de columnas como input y pull down
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    for (int i = 0; i < 4; i++) {
        GPIO_InitStruct.GPIO_Pin = miTeclado.columnas[i].pin;
        GPIO_Init(miTeclado.columnas[i].puerto, &GPIO_InitStruct);
    }

    currentState = STATE_DETECCION;
}

static char escanear_hardware(void) {
    for (int f = 0; f < 4; f++) {
        // Reset de todas las filas
        for (int i = 0; i < 4; i++) {
            GPIO_ResetBits(miTeclado.filas[i].puerto, miTeclado.filas[i].pin);
        }
        // Set de la fila actual
        GPIO_SetBits(miTeclado.filas[f].puerto, miTeclado.filas[f].pin);

        // lee columnas
        for (int c = 0; c < 4; c++) {
            if (GPIO_ReadInputDataBit(miTeclado.columnas[c].puerto, miTeclado.columnas[c].pin) == Bit_SET) {
                return mapaTeclas[f][c];
            }
        }
    }
    return 0;
}

char teclado_fsm_update(void) {
    char teclaActual = 0;
    char teclaValidada = 0;

    switch(currentState) {
        case STATE_DETECCION:
            teclaActual = escanear_hardware();
            if (teclaActual != 0) {
                teclaGuardada = teclaActual;
                currentState = STATE_DEBOUNCE;
                contadorTicks = 0;
            }
            break;

        case STATE_DEBOUNCE:
            contadorTicks++;
            if (contadorTicks >= 20) { // 20 llamadas a la FSM para el antirrebote
                currentState = STATE_CONFIRMACION;
            }
            break;

        case STATE_CONFIRMACION:
            if (escanear_hardware() == teclaGuardada) {
                teclaValidada = teclaGuardada;
                currentState = STATE_ESPERO_LIBERACION;
            } else {
                currentState = STATE_DETECCION;
            }
            break;

        case STATE_ESPERO_LIBERACION:
            if (escanear_hardware() == 0) {
                currentState = STATE_DETECCION;
            }
            break;

        default:
            currentState = STATE_DETECCION;
            break;
    }
    return teclaValidada;
}
