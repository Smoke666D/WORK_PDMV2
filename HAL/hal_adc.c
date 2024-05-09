/*
 * hal_adc.c
 *
 *  Created on: 11 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_adc.h"
#if MCU == APM32
	#include "apm32f4xx_adc.h"
	#include "apm32f4xx_rcm.h"
	#include "apm32f4xx_dma.h"
#endif

static uint8_t ADC_INIT = 0;

#if MCU == APM32
static const  ADC_CHANNEL_T ADC_chennel_ref[]={  ADC_CHANNEL_0,  ADC_CHANNEL_1,  ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4,  ADC_CHANNEL_5,  ADC_CHANNEL_6,  ADC_CHANNEL_7,  ADC_CHANNEL_8,  ADC_CHANNEL_9,  ADC_CHANNEL_10,
    ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15,  ADC_CHANNEL_16,  ADC_CHANNEL_17, ADC_CHANNEL_18, } ;
#endif

#if MCU == CH32
static const  ADC_CHANNEL_T ADC_chennel_ref[]={  ADC_Channel_0,  ADC_Channel_1,  ADC_Channel_2, ADC_Channel_3, ADC_Channel_4,  ADC_Channel_5,  ADC_Channel_6,  ADC_Channel_7,  ADC_Channel_8,  ADC_Channel_9,  ADC_Channel_10,
		ADC_Channel_11, ADC_Channel_12, ADC_Channel_13, ADC_Channel_14, ADC_Channel_15,  ADC_Channel_16,  ADC_Channel_17} ;
#endif

void HAL_ADC_CommonConfig()
 {
#if MCU == APM32
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
#endif
#if MCU == CH32
	 RCC_ADCCLKConfig(RCC_PCLK2_Div8);
#endif
 }


void HAL_ADC_ContiniusScanCinvertionDMA( ADC_NUMBER_t adc, uint8_t channel_count, uint8_t *  channel_nmber)
 {
#if MCU == APM32
	ADC_Config_T  adcConfig;
	if ( adc == ADC_1)
	    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC1 );
	else
	if (adc == ADC_2)
	   RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC2 );
	else
	   RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_ADC3 );
#endif
#if MCU == CH32
	ADC_InitTypeDef  adcConfig;
	if ( adc == ADC_1)
		 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		else
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

#endif
	 HAL_ADC_CommonConfig();
#if MCU==CH32
	 ADC_DeInit(adc);
	 adcConfig.ADC_Mode = ADC_Mode_Independent;
	 adcConfig.ADC_ScanConvMode =ENABLE;
	 adcConfig.ADC_ContinuousConvMode = DISABLE;
	 adcConfig.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;
	 adcConfig.ADC_DataAlign = ADC_DataAlign_Right;
	 adcConfig.ADC_NbrOfChannel = ADC_CHANNEL;
	 adcConfig.ADC_OutputBuffer = ADC_OutputBuffer_Disable;
	 adcConfig.ADC_Pga = ADC_Pga_1;
	 ADC_Init(adc, &adcConfig);
	 for (u8 i=0; i< (channel_count) ;i++)
	 {
		 ADC_ConfigRegularChannel(adc,  ADC_chennel_ref[channel_nmber[ i  ]],  i + 1, ADC_SAMPLETIME_112CYCLES);
	 }

	 ADC_EnableEOCOnEachChannel( adc );
#endif
#if MCU == CH32
	 for (u8 i=0; i< (channel_count) ;i++)
	 {
		 ADC_RegularChannelConfig(adc,ADC_chennel_ref[channel_nmber[ i  ]], i + 1,  ADC_SampleTime_239Cycles5 );
	 }
	 ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	 ADC_DMACmd(adc, ENABLE);
	 ADC_Cmd(adc, ENABLE);
	 ADC_BufferCmd(adc, DISABLE); //disable buffer
	 ADC_ResetCalibration(adc);
	 while(ADC_GetResetCalibrationStatus(adc));
	 ADC_StartCalibration(adc);
	 while(ADC_GetCalibrationStatus(adc));
	 Calibrattion_Val = Get_CalibrationValue(adc);
	 ADC_BufferCmd(adc, ENABLE);
#endif

 }


void HAL_ADC_TempEnable()
{
#if MCU == APM32
	 ADC_EnableTempSensorVrefint();
#endif
}

void HAL_ADC_VrefEnable()
{
#if MCU == APM32
	  ADC_EnableVbat();
#endif
}




void HAL_ADC_Enable(ADC_NUMBER_t adc_number)
{
#if MCU == APM32
	ADC_Enable(adc_number);
#endif
}


void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number)
{
#if MCU == APM32
	ADC_Disable(adc_number);
	ADC_DisableDMA(adc_number);
#endif
}

