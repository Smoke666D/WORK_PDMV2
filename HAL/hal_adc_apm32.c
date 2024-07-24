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

static ADC_T * ADC_REF[3] = {ADC1, ADC2,ADC3 };

static const  ADC_CHANNEL_T ADC_chennel_ref[]={  ADC_CHANNEL_0,  ADC_CHANNEL_1,  ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4,  ADC_CHANNEL_5,  ADC_CHANNEL_6,  ADC_CHANNEL_7,  ADC_CHANNEL_8,  ADC_CHANNEL_9,  ADC_CHANNEL_10,
    ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15,  ADC_CHANNEL_16,  ADC_CHANNEL_17, ADC_CHANNEL_18, } ;
static const RCM_APB2_PERIPH_T ADC_APB[]={RCM_APB2_PERIPH_ADC1 ,RCM_APB2_PERIPH_ADC2, RCM_APB2_PERIPH_ADC3};



void HAL_ADC_CommonConfig()
 {
	 RCM->APB2RST |= RCM_APB2_PERIPH_ADC;            // Enable the High Speed APB (APB2) peripheral reset
	 RCM->APB2RST &= (uint32_t)~RCM_APB2_PERIPH_ADC; //  Disable the High Speed APB (APB2) peripheral reset
	 ADC->CCTRL_B.ADCMSEL = ADC_MODE_INDEPENDENT;     //Config the ADCs peripherals
	 ADC->CCTRL_B.ADCPRE =  ADC_PRESCALER_DIV8;
	 ADC->CCTRL_B.DMAMODE = ADC_ACCESS_MODE_DISABLED;
	 ADC->CCTRL_B.SMPDEL2 = ADC_TWO_SAMPLING_5CYCLES;
 }



static void HAL_ADC_ConfigRegularChannel(ADC_NUMBER_t adc, uint8_t channel, uint8_t rank, uint8_t sampleTime)
{
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;

    if (channel > ADC_CHANNEL_9)
    {
        temp1 = ADC_REF[adc]->SMPTIM1;
        temp2 = (uint32_t)7 << (3 * (channel - 10));
        temp1 &= ~temp2;
        temp2 = (uint32_t)sampleTime << (3 * (channel - 10));
        temp1 |= temp2;
        ADC_REF[adc]->SMPTIM1 = temp1;
    }
    else
    {
        temp1 = ADC_REF[adc]->SMPTIM2;
        temp2 = (uint32_t)7 << (3 * channel);
        temp1 &= ~temp2;
        temp2 = (uint32_t)sampleTime << (3 * channel);
        temp1 |= temp2;
        ADC_REF[adc]->SMPTIM2 = temp1;
    }

    if (rank < 7)
    {
        temp1 = ADC_REF[adc]->REGSEQ3;
        temp2 = (uint32_t)0x1F << (5 * (rank - 1));
        temp1 &= ~temp2;
        temp2 = (uint32_t)channel << (5 * (rank - 1));
        temp1 |= temp2;
        ADC_REF[adc]->REGSEQ3 = temp1;
    }
    else if (rank < 13)
    {
        temp1 = ADC_REF[adc]->REGSEQ2;
        temp2 = (uint32_t)0x1F << (5 * (rank - 7));
        temp1 &= ~temp2;
        temp2 = (uint32_t)channel << (5 * (rank - 7));
        temp1 |= temp2;
        ADC_REF[adc]->REGSEQ2 = temp1;
    }
    else
    {
        temp1 = ADC_REF[adc]->REGSEQ1;
        temp2 = (uint32_t)0x1F << (5 * (rank - 13));
        temp1 &= ~temp2;
        temp2 = (uint32_t)channel << (5 * (rank - 13));
        temp1 |= temp2;
        ADC_REF[adc]->REGSEQ1 = temp1;
    }
}


void HAL_ADC_ContiniusScanCinvertionDMA( ADC_NUMBER_t adc, uint8_t channel_count, uint8_t *  channel_nmber)
 {

	RCM->APB2CLKEN |= ADC_APB[adc];
	ADC_REF[adc]->CTRL1_B.RESSEL = ADC_RESOLUTION_12BIT;
	ADC_REF[adc]->CTRL1_B.SCANEN = ENABLE;
	ADC_REF[adc]->CTRL2_B.REGEXTTRGEN = ADC_EXT_TRIG_EDGE_NONE;
	ADC_REF[adc]->CTRL2_B.REGEXTTRGSEL =  ADC_EXT_TRIG_CONV_TMR1_CC1;
	ADC_REF[adc]->CTRL2_B.DALIGNCFG = ADC_DATA_ALIGN_RIGHT;
	ADC_REF[adc]->CTRL2_B.CONTCEN = ENABLE;
	ADC_REF[adc]->REGSEQ1_B.REGSEQLEN = channel_count - 1;
	for (u8 i=0; i< (channel_count) ;i++)
	{
		 HAL_ADC_ConfigRegularChannel(adc,  ADC_chennel_ref[channel_nmber[ i  ]],  i + 1, ADC_SAMPLETIME_112CYCLES);
	}
	ADC_REF[adc]->CTRL2_B.EOCSEL = BIT_SET; // Enables the EOC on each regular channel conversion
 }

/*
 * Включение термодатчика
 */
void HAL_ADC_TempEnable()
{
	 ADC->CCTRL_B.TSVREFEN = BIT_SET;  //Enables the temperature sensor and Vrefint channels.
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
	ADC_REF[adc_number]->CTRL2_B.ADCEN = BIT_SET;  //Enables the specified ADC peripheral.
}

void HAL_ADCDMA_Disable(ADC_NUMBER_t adc_number)
{
	ADC_REF[adc_number]->CTRL2_B.ADCEN = BIT_RESET; //Disable the specified ADC peripheral.
	ADC_REF[adc_number]->CTRL2_B.DMAEN = BIT_RESET; // Disables the specified ADC DMA request.
}

#endif
