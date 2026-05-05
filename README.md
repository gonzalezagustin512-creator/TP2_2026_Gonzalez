# TP2_2026_Gonzalez
Trabajo Práctico Nº 2: GPIO &amp; Teclado
## Ejercicio 1b: pin de led ocnfigurado como open drain

observacion:
al configurar el pin PD10 como `GPIO_OType_OD`, el led no se enciende, pero el codigo sigue enviando el 1 para que se prenda.

explicacion de lo que pasa:
El modo OD  desactiva el transistor interior de la etapa de salida, el que se encarga de conectar el pin a VCC push pull. En este modo, el pin solo tiene dos estados posibles apagado o flotando.

como el led de la placa requiere que el microcontrolador le entregue voltaje para encender, al estar en Open Drain y poner un 1, no hay tension que alimente el circuito, por lo que el LED permanece apagado.