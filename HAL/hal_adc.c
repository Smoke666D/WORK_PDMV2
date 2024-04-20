/*
 * hal_adc.c
 *
 *  Created on: 11 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_adc.h"
#include "apm32f4xx_adc.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx_dma.h"


static uint8_t ADC_INIT = 0;
static const ADC_T*  ADC_ref[]={ADC1,ADC2,ADC3};

void HAL_ADC_CommonConfig()
 {
	 ADC_CommonConfig_T      adcCommonConfig;

	 if (ADC_INIT==0)
	 {
		 ADC_Reset();
		 ADC_CommonConfigStructInit(&adcCommonConfig);
		 adcCommonConfig.mode            = ADC_MODE_INDEPENDENT;
		 adcCommonConfig.prescaler       = ADC_PRESCALER_DIV8;
		 ADC_CommonConfig(&adcCommonConfig);
		 ADC_INIT = 1;
	 }
 }


void HAL_ADC_ContiniusScanCinvertioDMA( uint8_t ADC_NUMBER, uint8_t channel_count, uint8_t * channel_nmber)
 {
	 ADC_T* adc;
	 ADC_Config_T  adcConfig;
	 switch ( ADC_NUMBER )
	 {
	 	 case 1:
	 		 RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC1 );
	 		 adc = ADC1;
	 		 break;
	 	 case 2:
	 		 RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC2 );
	 		 adc = ADC2;
	 		 break;
	 	 case 3:
	 		 RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC3 );
	 		 adc = ADC3;
	 		 break;
	 	 default:
	 		 return;
	 }
	 HAL_ADC_CommonConfig();
	 ADC_ConfigStructInit( &adcConfig );
	 adcConfig.resolution            = ADC_RESOLUTION_12BIT;
	 adcConfig.scanConvMode          = ENABLE;
	 adcConfig.continuousConvMode    = ENABLE;
	 adcConfig.dataAlign             = ADC_DATA_ALIGN_RIGHT;
	 adcConfig.extTrigEdge           = ADC_EXT_TRIG_EDGE_NONE;
	 adcConfig.nbrOfChannel          = channel_count;
	 ADC_Config(adc, &adcConfig);
	 for (u8 i=0; i< (channel_count) ;i++)
	 {
		 ADC_ConfigRegularChannel(adc,channel_nmber[ i  ],  i + 1, ADC_SAMPLETIME_112CYCLES);
	 }
	 ADC_EnableEOCOnEachChannel( adc );
 }


void HAL_ADC_TempEnable()
{
	 ADC_EnableTempSensorVrefint();

}

void HAL_ADC_VrefEnable()
{
	  ADC_EnableVbat();
}


void HAL_ADCInit()
{





}


void HAL_ADC_Enable(ADC_NUMBER_t adc_number)
{
	ADC_Enable(ADC_ref[adc_number]);
}


void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number)
{
	ADC_Disable(ADC_ref[adc_number]);
	ADC_DisableDMA(ADC_ref[adc_number]);
}

