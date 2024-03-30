/*******************************************************************************
* Title                 :   gpio
* Filename              :   gpio.c
* Author                :   Javier Perez Macias
* Origin Date           :   08/03/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
* Description           : Este programa consiste en el uso de ADC, GPIO y UART
* usando los drivers de Mediatek junto al FreeRTOS.
* Codigo hecho en base a los ejemplos proveidos por MediaTek Labs:
* https://github.com/MediaTek-Labs/mt3620_m4_software/
******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "gpio.h"

/******************************************************************************
* Definicion de variables
*******************************************************************************/
static os_hal_gpio_data buttonStateA = PRESSED, buttonStateB = PRESSED;

/******************************************************************************
* Definicion de Funciones
******************************************************************************/

/*!
* Funcion: gpio_Init
* Pre-condiciones: Ninguna
* Descripcion: Inicializacion del GPIO
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void gpio_Init(void) {
	/* Inicializacion del GPIO */
	mtk_os_hal_gpio_set_direction(LED1_RED, OUTPUT);
	mtk_os_hal_gpio_set_direction(LED1_GREEN, OUTPUT);
	mtk_os_hal_gpio_set_direction(LED1_BLUE, OUTPUT);
	mtk_os_hal_gpio_set_direction(BUTTON_A, INPUT);
	mtk_os_hal_gpio_set_direction(BUTTON_B, INPUT);
}


/*!
* Funcion: gpio_led1_change_color
* Pre-condiciones: Haber inicializado el LED
* Descripcion: Cambia el color del led 1
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
static void gpio_led1_change_color(os_hal_gpio_data R, os_hal_gpio_data G, os_hal_gpio_data B){
	// 0 = Encencido
	// 1 = Apagado
	mtk_os_hal_gpio_set_output(LED1_RED, R);
	mtk_os_hal_gpio_set_output(LED1_GREEN, G);
	mtk_os_hal_gpio_set_output(LED1_BLUE, B);
}


/*!
* Funcion: gpio_task
* Pre-condiciones: Haber inicializado los puertos a utilizar del GPIO
* Descripcion: Hilo del RTOS, detecta las pulsaciones de los botones, lo refleja enviando un mensaje por UART 
	y cambiando de color el LED1
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void gpio_task(void* pParameters){
	os_hal_gpio_data A,B = 0;

	printf("GPIO Task Started\n");

	while (1) {
		/* Captura el estado del boton A. */
		mtk_os_hal_gpio_get_input(BUTTON_A, &A);
		if (A != buttonStateA) { //Botones con logica pullup
			if (A == PRESSED) {
				printf("Boton A\n");
			}
			buttonStateA = A;
		}

		/* Captura el estado del boton B. */
		//Botones con logica pullup
		mtk_os_hal_gpio_get_input(BUTTON_B, &B);
		if (B != buttonStateB) { 
			if (B == PRESSED) {
				printf("Boton B\n");
			}
			buttonStateB = B;
		}

		gpio_led1_change_color(OFF & A, OFF & B, OFF & A);

		/* Retraso de 100ms */
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
/*************** FIN DEL ARCHIVO ***************************************************************************/