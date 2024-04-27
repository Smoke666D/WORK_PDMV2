/*
 * hal_dma.c
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_dma.h"
#include "apm32f4xx_dma.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx_adc.h"


DMA_CFG_t DMA_CALLback[7]   __SECTION(RAM_SECTION_CCMRAM);


void HAL_DMA2InitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, DMA_CHANNEL_t channel, void (*f)(void))
{
	   DMA_Config_T dmaConfig;
	   DMA_Stream_T* dmastream;
	   uint32_t flag;
	   IRQn_Type  irq;
	   DMA_CHANNEL_T ch;
	  /* Enable DMA clock */
	   RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_DMA2);
	   switch (direction)
	   {
	   	   case MTOP:
	   		   dmaConfig.dir =  DMA_DIR_MEMORYTOPERIPHERAL;
	   		   break;
	   	   case PTOM:
	   		   dmaConfig.dir = DMA_DIR_PERIPHERALTOMEMORY;
	   		   break;
	   }
	   switch (dma_size)
	   {
	   	   case DMA_BYTE:
	   		   dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
	   		   dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
	   		   break;
	   	   case DMA_HWORD:
	   		   dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_HALFWORD;
	   		   dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_HALFWORD;
	   		   break;
	   	   case DMA_WORD:
	   		   dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_WORD;
	   		   dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_WORD;
	   		   break;
	   }
	   switch (channel)
	   {
	   	   case DMA_CH_0:
	   		   ch =DMA_CHANNEL_0;

			   break;
	   	   case DMA_CH_1:
	   		   ch =DMA_CHANNEL_1;

	   		   break;
	   	   case DMA_CH_2:
	   		   ch =DMA_CHANNEL_2;

	   		   break;
	   	   case DMA_CH_3:
	   		   ch =DMA_CHANNEL_3;

	   		   break;
	   	case DMA_CH_4:
	   		   ch =DMA_CHANNEL_4;

	   		   break;
	   	case DMA_CH_5:
	   		   ch =DMA_CHANNEL_5;

	   		   break;
	     case DMA_CH_6:
	   		   ch =DMA_CHANNEL_6;

	   		   break;
	   	case DMA_CH_7:
	   		   ch =DMA_CHANNEL_7;

	   		   break;
	   	default:
	   		return;
	   }
	   switch ( stream )
	   {
	   	   case Stream_0:
	   		   dmastream = DMA2_Stream0;
	   		   flag = DMA_INT_TCIFLG0;
	   		   irq = DMA2_STR0_IRQn;
	   		   DMA_CALLback[0].CallBack = f;
	   		   break;
	   	   case Stream_1:
	   		   dmastream = DMA2_Stream1;
	   		   flag = DMA_INT_TCIFLG1;
	   		   irq = DMA2_STR1_IRQn;
	   		   DMA_CALLback[1].CallBack = f;
	   		   break;
	   	   case Stream_2:
	   		   dmastream = DMA2_Stream2;
	   		   flag = DMA_INT_TCIFLG2;
	   		   irq = DMA2_STR2_IRQn;
	   		   DMA_CALLback[2].CallBack = f;
	   		   break;
	   	   case Stream_3:
	   		   dmastream = DMA2_Stream3;
	   		   flag = DMA_INT_TCIFLG3;
	   		   irq = DMA2_STR3_IRQn;
	   		   DMA_CALLback[3].CallBack = f;
	   		   break;
	   	   case Stream_4:
	   		   dmastream = DMA2_Stream4;
	   		   flag = DMA_INT_TCIFLG4;
	   		   irq = DMA2_STR4_IRQn;
	   		   DMA_CALLback[4].CallBack = f;
	   		   break;
	   	   case Stream_5:
	   		   dmastream = DMA2_Stream5;
	   		   flag = DMA_INT_TCIFLG5;
	   		   irq = DMA2_STR5_IRQn;
	   		   DMA_CALLback[5].CallBack = f;
	   		   break;
	   	   case Stream_6:
	   		   dmastream = DMA2_Stream6;
	   		   flag = DMA_INT_TCIFLG6;
	   		   irq = DMA2_STR6_IRQn;
	   		   DMA_CALLback[6].CallBack = f;
	   		   break;
	   	   case Stream_7:
	   		   dmastream = DMA2_Stream7;
	   		   flag = DMA_INT_TCIFLG7;
	   		   irq = DMA2_STR7_IRQn;
	   		   DMA_CALLback[7].CallBack = f;
	   		   break;
	   	   default:
	   		   return;
	   }
	   dmaConfig.memoryInc		   = DMA_MEMORY_INC_ENABLE;
	   dmaConfig.peripheralInc 	   = DMA_PERIPHERAL_INC_DISABLE;
	   dmaConfig.loopMode 		   = DMA_MODE_NORMAL;
	   dmaConfig.priority 		   = DMA_PRIORITY_LOW;
	   dmaConfig.fifoMode          = DMA_FIFOMODE_DISABLE;
	   dmaConfig.fifoThreshold     = DMA_FIFOTHRESHOLD_FULL;
	   dmaConfig.peripheralBurst   = DMA_PERIPHERALBURST_SINGLE;
	   dmaConfig.memoryBurst       = DMA_MEMORYBURST_SINGLE;
	   dmaConfig.peripheralBaseAddr = paddr;
	   dmaConfig.channel           = ch;
	   DMA_Config(dmastream, &dmaConfig);
	   DMA_Enable(dmastream);
	   DMA_ClearIntFlag(DMA2_Stream4, flag);
	   NVIC_EnableIRQRequest( irq, 6, 0 );

}



void HAL_ADC_StartDMA( DMA_Stram_t chanel, uint16_t * data, uint16_t size)
{
	ADC_T* adc;
	DMA_Stream_T* stream;
	switch (chanel)
	{
		case Stream_4:
			stream = DMA2_Stream4;
			DMA_ClearStatusFlag(DMA2_Stream4, DMA_FLAG_TEIFLG4 | DMA_FLAG_DMEIFLG4 );
			adc = ADC1;

			break;
		case Stream_2:
			stream = DMA2_Stream2;
			DMA_ClearStatusFlag(DMA2_Stream2, DMA_FLAG_TEIFLG2 | DMA_FLAG_DMEIFLG2 );
			adc = ADC2;

			break;
		case Stream_0:
			stream = DMA2_Stream0;
			DMA_ClearStatusFlag(DMA2_Stream0, DMA_FLAG_TEIFLG0 | DMA_FLAG_DMEIFLG0 );
			adc = ADC3;
			break;
		default:
			return;
	}
	DMA_ConfigDataNumber(stream, size);
	DMA_ConfigMemoryTarget(stream, data, DMA_MEMORY_0);
	ADC_ClearStatusFlag(adc, ADC_FLAG_EOC | ADC_FLAG_OVR);
	ADC_EnableDMA(adc);
	DMA_EnableInterrupt(stream, DMA_INT_TCIFLG);
	DMA_Enable(stream);
	ADC_SoftwareStartConv(adc);
}


void DMA2_STR4_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA2_Stream4, DMA_INT_TCIFLG4) ==SET)
	{
		DMA_ClearIntFlag(DMA2_Stream4, DMA_INT_TCIFLG4);
		DMA_DisableInterrupt(DMA2_Stream4, DMA_INT_TCIFLG);
		DMA_CALLback[4].CallBack();
	}
}

void DMA2_STR2_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA2_Stream2, DMA_INT_TCIFLG2) ==SET)
	{
		DMA_DisableInterrupt(DMA2_Stream2, DMA_INT_TCIFLG);
		DMA_ClearIntFlag(DMA2_Stream2, DMA_INT_TCIFLG2);
		DMA_CALLback[2].CallBack();
	}
}
void DMA2_STR0_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA2_Stream0, DMA_INT_TCIFLG0) ==SET)
	{
		DMA_DisableInterrupt(DMA2_Stream0, DMA_INT_TCIFLG);
		DMA_ClearIntFlag(DMA2_Stream0, DMA_INT_TCIFLG0);
		DMA_CALLback[0].CallBack();
	}
}

