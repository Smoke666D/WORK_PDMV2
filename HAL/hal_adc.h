/*
 * hal_adc.h
 *
 *  Created on: 11 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_ADC_H_
#define HAL_HAL_ADC_H_


#include "main.h"



typedef enum {
	ADC_1 = 0,
	ADC_2 = 1,
	ADC_3 = 2
} ADC_NUMBER_t;

void HAL_ADC_CommonConfig();
void HAL_ADC_ContiniusScanCinvertioDMA( uint8_t ADC_NUMBER, uint8_t channel_count, uint8_t * channel_nmber);
void HAL_ADC_TempEnable();
void HAL_ADC_VrefEnable();
void HAL_ADC_Enable(ADC_NUMBER_t adc_number);
void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number);

#endif /* HAL_HAL_ADC_H_ */
