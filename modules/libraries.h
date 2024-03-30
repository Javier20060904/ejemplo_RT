/*******************************************************************************
* Title                 :   libraries
* Filename              :   libraries.h
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

#ifndef LIBRARIES_H
#define LIBRARIES_H

/******************************************************************************
* Includes
*******************************************************************************/
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "printf.h"
#include "mt3620.h"
#include "os_hal_uart.h"
#include "os_hal_gpio.h"
#include "os_hal_adc.h"

#endif
/*************** FIN DEL ARCHIVO ***************************************************************************/
