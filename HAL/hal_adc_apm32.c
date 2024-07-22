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

static const  ADC_CHANNEL_T ADC_chennel_ref[]={  ADC_CHANNEL_0,  ADC_CHANNEL_1,  ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4,  ADC_CHANNEL_5,  ADC_CHANNEL_6,  ADC_CHANNEL_7,  ADC_CHANNEL_8,  ADC_CHANNEL_9,  ADC_CHANNEL_10,
    ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15,  ADC_CHANNEL_16,  ADC_CHANNEL_17, ADC_CHANNEL_18, } ;


void HAL_ADC_CommonConfig()
 {
	 RCM->APB2RST |= RCM_APB2_PERIPH_ADC;            // Enable the High Speed APB (APB2) peripheral reset
	 RCM->APB2RST &= (uint32_t)~RCM_APB2_PERIPH_ADC; //  Disable the High Speed APB (APB2) peripheral reset
	 ADC->CCTRL_B.ADCMSEL = ADC_MODE_INDEPENDENT;     //Config the ADCs peripherals
	 ADC->CCTRL_B.ADCPRE =  ADC_PRESCALER_DIV8;
	 ADC->CCTRL_B.DMAMODE = ADC_ACCESS_MODE_DISABLED;
	 ADC->CCTRL_B.SMPDEL2 = ADC_TWO_SAMPLING_5CYCLES;
 }

void HAL_ADC_ContiniusScanCinvertionDMA( ADC_NUMBER_t adc, uint8_t channel_count, uint8_t *  channel_nmber)
 {
	if ( adc == ADC_1)
		RCM->APB2CLKEN |=  RCM_APB2_PERIPH_ADC1 ;
	else
	if (adc == ADC_2)
		RCM->APB2CLKEN |= RCM_APB2_PERIPH_ADC2 ;
	else
		RCM->APB2CLKEN |= RCM_APB2_PERIPH_ADC3 ;

	 adc->CTRL1_B.RESSEL = ADC_RESOLUTION_12BIT;
	 adc->CTRL1_B.SCANEN = ENABLE;

	 adc->CTRL2_B.REGEXTTRGEN = ADC_EXT_TRIG_EDGE_NONE;
	 adc->CTRL2_B.REGEXTTRGSEL =  ADC_EXT_TRIG_CONV_TMR1_CC1;
	 adc->CTRL2_B.DALIGNCFG = ADC_DATA_ALIGN_RIGHT;
	 adc->CTRL2_B.CONTCEN = ENABLE;
	 adc->REGSEQ1_B.REGSEQLEN = channel_count - 1;

	 for (u8 i=0; i< (channel_count) ;i++)
	 {
		 ADC_ConfigRegularChannel(adc,  ADC_chennel_ref[channel_nmber[ i  ]],  i + 1, ADC_SAMPLETIME_112CYCLES);
	 }
	 adc->CTRL2_B.EOCSEL = BIT_SET; // Enables the EOC on each regular channel conversion
 }

/*
 * Включение термодатчика
 */
void HAL_ADC_TempEnable()
{
	 ADC->CCTRL_B.TSVREFEN = BIT_SET;  //Enables the temperature sensor and Vrefint channels.
}


void HAL_ADC_VrefEnable()
{
	  ADC_EnableVbat();
}

void HAL_ADC_StartDMA( DMA_Stram_t chanel, uint16_t * data, uint16_t size)
{
	ADC_T* adc;
	if (chanel == DMA2_CH4)
	{
		    DMA2->HIFCLR = ((DMA_FLAG_TEIFLG4 | DMA_FLAG_DMEIFLG4 ) & 0x0F7D0F7D); //Clears the DMAy channelx's pending flags.
			adc = ADC1;
	}
	else if (chanel == DMA2_CH2)
	{
		    DMA2->LIFCLR =((DMA_FLAG_TEIFLG2 | DMA_FLAG_DMEIFLG2 ) & 0x0F7D0F7D); //Clears the DMAy channelx's pending flags.
			adc = ADC2;
	}
	else if (chanel == DMA2_CH0)
    {
		    DMA2->LIFCLR =((DMA_FLAG_TEIFLG0 | DMA_FLAG_DMEIFLG0 ) & 0x0F7D0F7D); //Clears the DMAy channelx's pending flags.
			adc = ADC3;
	}
	HAL_DMA_SerSource( chanel,  data, size );
	adc->STS = ~(uint32_t)(ADC_FLAG_EOC | ADC_FLAG_OVR);  // Clears the pending ADC flag
	adc->CTRL2_B.DMAEN = BIT_SET;   // Enables the specified ADC DMA request.
	HAL_DMA_TCEnable(chanel);
	adc->CTRL2_B.REGSWSC = BIT_SET; // Enables the selected ADC software start conversion of the regular channels.
}


void HAL_ADC_Enable(ADC_NUMBER_t adc_number)
{
	adc_number->CTRL2_B.ADCEN = BIT_SET;  //Enables the specified ADC peripheral.
}

void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number)
{
	adc_number->CTRL2_B.ADCEN = BIT_RESET; //Disable the specified ADC peripheral.
	adc_number->CTRL2_B.DMAEN = BIT_RESET; // Disables the specified ADC DMA request.
}

#endif
