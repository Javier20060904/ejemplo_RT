/*******************************************************************************
* Title                 :   gpio
* Filename              :   gpio.h
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

#ifndef GPIO_H
#define GPIO_H

/******************************************************************************
* Includes
*******************************************************************************/

#include "libraries.h"

/******************************************************************************
* Constantes del preprocesador
*******************************************************************************/

#define ON OS_HAL_GPIO_DATA_LOW
#define OFF OS_HAL_GPIO_DATA_HIGH
#define OUTPUT OS_HAL_GPIO_DIR_OUTPUT
#define INPUT OS_HAL_GPIO_DIR_INPUT
#define PRESSED OS_HAL_GPIO_DATA_LOW

#define LED1_RED OS_HAL_GPIO_8
#define LED1_GREEN OS_HAL_GPIO_9
#define LED1_BLUE OS_HAL_GPIO_10
#define BUTTON_A OS_HAL_GPIO_12
#define BUTTON_B OS_HAL_GPIO_13

/******************************************************************************
* Prototipos de funciones
*******************************************************************************/
void gpio_task(void* pParameters);
void gpio_Init(void);

#endif
/*************** FIN DEL ARCHIVO ***************************************************************************/