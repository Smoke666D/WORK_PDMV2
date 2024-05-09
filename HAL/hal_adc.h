/*
 * hal_adc.h
 *
 *  Created on: 11 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_ADC_H_
#define HAL_HAL_ADC_H_


#include "main.h"
#include "hal_config.h"

typedef enum
{
    ADC_CH_0 = 0,  /*!< ADC Channel 0 */
    ADC_CH_1 = 1,  /*!< ADC Channel 1 */
    ADC_CH_2 = 2,  /*!< ADC Channel 2 */
    ADC_CH_3 = 3,  /*!< ADC Channel 3 */
    ADC_CH_4 = 4,  /*!< ADC Channel 4 */
    ADC_CH_5 = 5,  /*!< ADC Channel 5 */
    ADC_CH_6 = 6,  /*!< ADC Channel 6 */
    ADC_CH_7 = 7,  /*!< ADC Channel 7 */
    ADC_CH_8 = 8,  /*!< ADC Channel 8 */
    ADC_CH_9 = 9,  /*!< ADC Channel 9 */
    ADC_CH_10 = 10, /*!< ADC Channel 10 */
    ADC_CH_11 = 11, /*!< ADC Channel 11 */
    ADC_CH_12 = 12, /*!< ADC Channel 12 */
    ADC_CH_13 = 13, /*!< ADC Channel 13 */
    ADC_CH_14 = 14, /*!< ADC Channel 14 */
    ADC_CH_15 = 15, /*!< ADC Channel 15 */
    ADC_CH_16 = 16, /*!< ADC Channel 16 */
    ADC_CH_17 = 17, /*!< ADC Channel 17 */
    ADC_CH_18 = 18, /*!< ADC Channel 18 */
} ADC_CH_T;




#if MCU == APM32
	#define ADC_NUMBER_t ADC_T*
	#define	ADC_1  ADC1
	#define ADC_2  ADC2
	#define ADC_3  ADC3
#endif
#if MCU == CH32
	#define ADC_NUMBER_t ADC_TypeDef*
	#define	ADC_1  ADC1
	#define ADC_2  ADC2
#endif


void HAL_ADC_CommonConfig();
void HAL_ADC_ContiniusScanCinvertionDMA( ADC_NUMBER_t adc, uint8_t channel_count, uint8_t * channel_nmber);
void HAL_ADC_TempEnable();
void HAL_ADC_VrefEnable();
void HAL_ADC_Enable(ADC_NUMBER_t adc_number);
void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number);

#endif /* HAL_HAL_ADC_H_ */
