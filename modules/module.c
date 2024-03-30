/*******************************************************************************
* Title                 :   module
* Filename              :   module.c
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
#include "module.h"

/******************************************************************************
* Definicion de Variables
******************************************************************************/
const UART_PORT uart_port_num = OS_HAL_UART_ISU0;

/******************************************************************************
* Definicion de Funciones
******************************************************************************/

/*!
* Funcion: vApplicationStackOverflowHook
* Pre-condiciones: Ninguno
* Descripcion: "Hook" para hacer stack over flow
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	printf("%s: %s\n", __func__, pcTaskName);
}

/*!
* Funcion: vApplicationMallocFailedHook
* Pre-condiciones: Ninguno
* Descripcion: "Hook" para la alocacion fallida de memoria
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void vApplicationMallocFailedHook(void)
{
	printf("%s\n", __func__);
}

/*!
* Funcion: _putchar
* Pre-condiciones: Ninguno
* Descripcion: "Hook" para printf
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void _putchar(char character)
{
	mtk_os_hal_uart_put_char(uart_port_num, character);
	if (character == '\n')
		mtk_os_hal_uart_put_char(uart_port_num, '\r');
}
/*************** FIN DEL ARCHIVO ***************************************************************************/