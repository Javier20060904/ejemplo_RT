/*******************************************************************************
* Title                 :   adc
* Filename              :   adc.c
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

/******************************************************************************
* Includes
*******************************************************************************/
#include "adc.h"

/******************************************************************************
* Declaracion de variables
*******************************************************************************/
unsigned int adc_rx_cnt;
u32* adc_rx_buf_periodic_mode;
u32* adc_rx_buf_one_shot_mode;

/******************************************************************************
* Definicion de Funciones	
******************************************************************************/

/*!
* Funcion: mtk_adc_periodic_mode_callback
* Pre-condiciones: Estar en uso el modo periodico del ADC
* Descripcion: Funcion callback del ADC en modo periodico
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void mtk_adc_periodic_mode_callback(void* data)
{
	/* Important Note! Don't do memory operations or heavy jobs in this callback function. */
	/* Memory operations or heavy jobs should be handled in the adc_task. */
	/* Keep only lightweight jobs in this callback function. */
	adc_rx_cnt++;
	if (adc_rx_cnt >= 4)
		adc_rx_cnt = 0;
}


/*!
* Funcion: mtk_adc_one_shot_mode_test
* Pre-condiciones: Ninguna
* Descripcion: Hace una lectura del ADC en modo one shot
* Valores de entrada: Ninguno
* Valores de salida: Finalizacion del proceso (-1 = Error)
*/
int mtk_adc_one_shot_mode_test(void)
{
	int ret = 0;
	int adc_no = 0;
	int channel_count = 0;
	u16 voltage = 0;
	struct adc_fsm_param adc_fsm_parameter; //Esturctura que caracteriza al ADC

	printf("\nADC en modo one shot:\n");

	ret = mtk_os_hal_adc_ctlr_init(); //Inicia el periferico en su estado original
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	//Configuracion del ADC
	adc_fsm_parameter.pmode = ADC_PMODE_ONE_TIME;
	adc_fsm_parameter.channel_map = BIT_MAP;
	/*	Se usa datos del First In Fisrt Out (FIFO)	*/
	adc_fsm_parameter.fifo_mode = ADC_FIFO_DIRECT; 
	adc_fsm_parameter.ier_mode = ADC_FIFO_IER_RXFULL;
	adc_fsm_parameter.vfifo_addr = adc_rx_buf_one_shot_mode; //Se usa los datos del RX
	adc_fsm_parameter.vfifo_len = CHANNEL_NUM;

	adc_fsm_parameter.rx_callback_func = NULL; //No tiene funcion callback
	adc_fsm_parameter.rx_callback_data = NULL;
	adc_fsm_parameter.rx_period_len = CHANNEL_NUM;

	ret = mtk_os_hal_adc_fsm_param_set(&adc_fsm_parameter); //Se asigna al periferico con la configuracion del ADC
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	ret = mtk_os_hal_adc_trigger_one_shot_once(); //Inicia el ADC en One Shot
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	channel_count = 0;
	for (adc_no = 0; adc_no < ADC_CHANNEL_MAX; adc_no++) {
		if ((adc_fsm_parameter.channel_map) & BIT(adc_no)) {
			voltage = (u32)((adc_rx_buf_one_shot_mode[channel_count] & ADC_DATA_MASK) //Lectura y conversion del voltaje
				>> ADC_DATA_BIT_OFFSET) * 2500 / 4096;
			printf("\tCH:%d(GPIO%d) voltaje: %dmv (%08X)\n", adc_no, ADC_GPIO_INDEX + adc_no,
				voltage, adc_rx_buf_one_shot_mode[channel_count]);
			channel_count++;
		}
	}

	ret = mtk_os_hal_adc_ctlr_deinit(); //Cuando el ADC ya no se usa se devuelve a su estado original
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}

/*!
* Funcion: mtk_adc_periodic_mode_test
* Pre-condiciones: Ninguna
* Descripcion: Realiza lecturas del ADC en modo periodico
* Valores de entrada: Ninguno
* Valores de salida: Finalizacion del proceso (-1 = Error)
*/
int mtk_adc_periodic_mode_test(void)
{
	int ret = 0, timeout_count = 0;
	unsigned int adc_rx_cnt_old = 0;
	struct adc_fsm_param adc_fsm_parameter;

	printf("\nADC en modo periodico:\n");

	ret = mtk_os_hal_adc_ctlr_init(); //Inicia el periferico en su estado original
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	//Configuracion del ADC en modo periodico
	adc_fsm_parameter.pmode = ADC_PMODE_PERIODIC;
	adc_fsm_parameter.channel_map = BIT_MAP;
	adc_fsm_parameter.sample_rate = SAMPLE_RATE;

	//Utiliza datos del Direct Memory Acces (DMA)
	adc_fsm_parameter.fifo_mode = ADC_FIFO_DMA;
	adc_fsm_parameter.ier_mode = ADC_FIFO_IER_RXFULL;
	adc_fsm_parameter.vfifo_addr = adc_rx_buf_periodic_mode;
	adc_fsm_parameter.vfifo_len = PERIODIC_BUF_LENGTH;

	//Se le asigna una funcion callback
	adc_fsm_parameter.rx_period_len = PERIODIC_BUF_LENGTH_PERIOD;
	adc_fsm_parameter.rx_callback_func = mtk_adc_periodic_mode_callback;
	adc_fsm_parameter.rx_callback_data = NULL;

	//Se configura los parametros del ADC.
	ret = mtk_os_hal_adc_fsm_param_set(&adc_fsm_parameter);
	if (ret) {
		printf("Func:%s, line:%d fail (ret=%d)\r\n", __func__, __LINE__, ret);
		return -1;
	}

	printf(COLOR_YELLOW"\t[Buf#]\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t11\t12\t13\t14\t15\n"COLOR_NONE);
	printf(COLOR_YELLOW"\t[ADC#]\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\tADC0\t"
		"ADC0\tADC0\tADC0\n"COLOR_NONE);

	adc_rx_cnt = 0;
	adc_rx_cnt_old = 0;
	mtk_os_hal_adc_period_start();
	while (1) {
		vTaskDelay(pdMS_TO_TICKS(1));
		if (adc_rx_cnt != adc_rx_cnt_old) {
			printf("\t[%d]\t%s%ldmv\t%ldmv\t%ldmv\t%ldmv\t%s%ldmv\t%ldmv\t%ldmv\t%ldmv\t%s%ldmv\t%ldmv\t"
				"%ldmv\t%ldmv\t%s%ldmv\t%ldmv\t%ldmv\t%ldmv%s\n",
				timeout_count,
				adc_rx_cnt == 1 ? COLOR_YELLOW : COLOR_NONE,
				((adc_rx_buf_periodic_mode[0] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[1] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[2] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[3] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				adc_rx_cnt == 2 ? COLOR_YELLOW : COLOR_NONE,
				((adc_rx_buf_periodic_mode[4] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[5] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[6] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[7] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				adc_rx_cnt == 3 ? COLOR_YELLOW : COLOR_NONE,
				((adc_rx_buf_periodic_mode[8] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[9] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[10] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[11] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				adc_rx_cnt == 0 ? COLOR_YELLOW : COLOR_NONE,
				((adc_rx_buf_periodic_mode[12] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[13] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[14] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				((adc_rx_buf_periodic_mode[15] & ADC_DATA_MASK) >> ADC_DATA_BIT_OFFSET) * 2500 / 4096,
				COLOR_NONE);
			adc_rx_cnt_old = adc_rx_cnt;
			timeout_count++;
		}

		if (timeout_count == 20)
			break;
	}

	ret = mtk_os_hal_adc_period_stop();
	if (ret)
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);

	ret = mtk_os_hal_adc_ctlr_deinit();
	if (ret) {
		printf("Func:%s, line:%d fail\r\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}

/*!
* Funcion: adc_task
* Pre-condiciones: Ninguna
* Descripcion: Hilo que se encarga de los procesos del ADC
* Valores de entrada: Ninguno
* Valores de salida: Ninguno
*/
void adc_task(void* pParameters)
{
	int ret = 0;

	//Si se desea usar el modo periodico, entonces quitar el comentado de esta seccion
	/* adc_rx_buf_periodic_mode = pvPortMalloc(sizeof(int) * PERIODIC_BUF_LENGTH);
	if (!adc_rx_buf_periodic_mode) {
		printf("ADC PERIOD MODE RX Malloc fail!\n");
		goto exit;
	}

	memset(adc_rx_buf_periodic_mode, 0, (sizeof(int) * PERIODIC_BUF_LENGTH));*/

	adc_rx_buf_one_shot_mode = pvPortMalloc(sizeof(int) * CHANNEL_NUM); //Se aloca memoria en Heap
	if (!adc_rx_buf_one_shot_mode) {
		printf("ADC ONE SHOT MODE RX Malloc fail!\n");
		vPortFree(adc_rx_buf_periodic_mode);
		goto exit;
	}

	memset(adc_rx_buf_one_shot_mode, 0, (sizeof(int) * CHANNEL_NUM));
	printf("Inicia el Hilo de ADC\n");

	while (1) {
		vTaskDelay(pdMS_TO_TICKS(1000));
		ret = mtk_adc_one_shot_mode_test();
		if (ret) {
			printf("Falla en el adc con one shot !\r\n");
			goto err_exit;
		}

		//Si se desea usar el modo periodico, entonces quitar el comentado de esta seccion
		/*vTaskDelay(pdMS_TO_TICKS(1000));
		ret = mtk_adc_periodic_mode_test();
		if (ret) {
			printf("mtk_adc_periodic_mode_test fail!\r\n");
			goto err_exit;
		}*/

		vTaskDelay(pdMS_TO_TICKS(2000));
	}

err_exit:
	//Si se desea usar el modo periodico, entonces quitar el comentado de esta seccion
	//vPortFree(adc_rx_buf_periodic_mode);
	vPortFree(adc_rx_buf_one_shot_mode);
exit:
	vTaskDelete(NULL);

}
/*************** FIN DEL ARCHIVO ***************************************************************************/