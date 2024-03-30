/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
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

#ifndef MODULE_H
#define MODULE_H

/******************************************************************************/
/* Includes */
/******************************************************************************/
#include "libraries.h"

/******************************************************************************
* Constantes del preprocesador
*******************************************************************************/
#define APP_STACK_SIZE_BYTES (1024 / 4)


/****************************************************************************
* Declaracion de variables
****************************************************************************/
extern const UART_PORT uart_port_num;

/****************************************************************************
* Prototipos de funciones
****************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName);
void vApplicationMallocFailedHook(void);
void _putchar(char character);
#endif