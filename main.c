/*******************************************************************************
* Title                 :   main
* Filename              :   main.c
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
#include "modules/module.h"
#include "modules/gpio.h"
#include "modules/adc.h"

/*!
* Funcion: RTCoreMain
* Pre-condiciones: Ninguna
* Descripcion: Funcion principal del programa
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
_Noreturn void RTCoreMain(void)
{
	/* Setup Vector Table */
	NVIC_SetupVectorTable();

	//Inicializa UART 
	mtk_os_hal_uart_ctlr_init(uart_port_num);

	//Inicializa los puertos GPIO
	gpio_Init();

	printf("\nIniciando ejemplo de FreeRTOS\n");

	//Crea el hilo para la funcion del GPIO
	xTaskCreate(gpio_task, "GPIO Task", APP_STACK_SIZE_BYTES, NULL, 2, NULL);
	//Crea el hilo para la funcion del ADC
	xTaskCreate(adc_task, "ADC Task", APP_STACK_SIZE_BYTES, NULL, 4, NULL);

	vTaskStartScheduler();
	for (;;)
		__asm__("wfi"); //Instruccion en ensamblador, en espera una interrupcion
}
/*************** FIN DEL ARCHIVO ***************************************************************************/