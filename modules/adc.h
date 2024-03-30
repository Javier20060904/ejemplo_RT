/*******************************************************************************
* Title                 :   adc
* Filename              :   adc.h
* Author                :   Javier Perez Macias
* Origin Date           :   11/03/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
* Description           : Este programa consiste en el uso de ADC, GPIO y UART
* usando los drivers de Mediatek junto al FreeRTOS.
* Codigo hecho en base a los ejemplos proveidos por MediaTek Labs:
* https://github.com/MediaTek-Labs/mt3620_m4_software/
******************************************************************************/

#ifndef ADC_H
#define ADC_H

/******************************************************************************
* Includes
*******************************************************************************/

#include "libraries.h"

/******************************************************************************
* Constantes del preprocesador
*******************************************************************************/

#define ADC_GPIO_INDEX				41		/* ADC0 = GPIO41 */
#define ADC_DATA_MASK				(BITS(4, 15))	/* ADC sample data mask (bit_4 ~ bit_15) */
#define ADC_DATA_BIT_OFFSET			4		/* ADC sample data bit offset */

#define SAMPLE_RATE			1	/* Total samples for "each channel" in one second. */
#define PERIODIC_BUF_LENGTH		16	/* 16 sampling data in the buffer. (16*4=64 Bytes) */
#define PERIODIC_BUF_LENGTH_PERIOD	4	/* Trigger callback whenever 4 sampling data are captured. */
#define BIT_MAP				0x1	/* ADC0 */
#define CHANNEL_NUM			1	/* ADC0 */

#define COLOR_YELLOW        "\033[1;33m"
#define COLOR_NONE          "\033[m"

/******************************************************************************
* Prototipos de funciones
*******************************************************************************/

void adc_task(void* pParameters);
void mtk_adc_periodic_mode_callback(void* data);
int mtk_adc_one_shot_mode_test(void);
int mtk_adc_periodic_mode_test(void);

#endif
/*************** FIN DEL ARCHIVO ***************************************************************************/