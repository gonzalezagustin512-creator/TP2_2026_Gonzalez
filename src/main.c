/*
 * Trabajo Práctico N°2: GPIO & Teclado
 * Asignatura: Técnicas Digitales II
 * Alumno: Agustín González
 * Institución: UTN - Facultad Regional Bahía Blanca
 */
#include "stm32f4xx.h"
#include "teclado.h"
#include "led.h"

//systick no va
/*
//variable cuenta los ms
volatile uint32_t msTicks = 0;

// temporizador integrado ejecuta cada 1ms
void SysTick_Handler(void) {
    msTicks++;
}
*/

// uso msticks como variable que se aumenta de a 1 en el while
uint32_t msTicks = 0;

int parpadeosRestantes = 0;//
int tiempoBase = 100;//tiempo que luego lo varian las letras

//delay para controlar el tiempo del ciclo
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (ms * 10000); i++) {
        __NOP();
    }
}

int main(void) {
    //no systick por ahora
    /*
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);
    }
    */

    // se sugirio q puede ir en la fsm pero por ahroa queda aca
    led_init();
    teclado_init();

    parpadeosRestantes = 0;

    while (1) {
        // Actualizar FSM del teclado
    	// se sugirio manera de mejorar esta parte pero aun no se relaiza el cambio
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

        // fsm led
        led_fsm_update();

        //se agrega el delay ms 1  (retardo blouqeante frena todas las acciones dle procesador por 1ms)
        //cada vuelta de while dura 1ms
        delay_ms(1);
        msTicks++; // incrementamos manualmente el contador de milisegundos
    }
}
