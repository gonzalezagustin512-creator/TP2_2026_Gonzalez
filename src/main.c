/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
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

int main(void) {
    char teclaDetectada;

    // Inicialización del sistema
    inicializarTeclado();
    // (También inicializa el pin del LED aquí como salida)

    while(1) {
        teclaDetectada = escanearTeclado(); // Le preguntamos al módulo qué pasa

        if (teclaDetectada == '1') {
            GPIO_SetBits(GPIOA, GPIO_Pin_5); // Encender LED (no importa si ya estaba prendido)
        }
        else if (teclaDetectada == '2') {
            GPIO_ResetBits(GPIOA, GPIO_Pin_5); // Apagar LED (no importa si ya estaba apagado)
        }

        // El resto de las teclas no tienen 'else', así que no hacen nada.
    }
}
