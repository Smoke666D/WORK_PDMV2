/*
 * hal_dma.c
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_dma.h"
#if MCU == APM32

	#include "apm32f4xx_rcm.h"
	#include "apm32f4xx_adc.h"
    #include "apm32f4xx.h"



DMA_CFG_t DMA_CALLback[7]   __SECTION(RAM_SECTION_CCMRAM);



void HAL_DMAInitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, uint32_t memadr, DMA_CHANNEL_t channel, uint8_t prior, uint8_t subprior, void (*f)(void))
{
	   DMA_Config_T dmaConfig;
	   uint32_t flag;
	   IRQn_Type  irq;
	   DMA_CHANNEL_T ch;
	  /* Enable DMA clock */
	   RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_DMA2);

	   dmaConfig.dir =  direction;

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
	   	   default:
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
	     default:
	   		   ch =DMA_CHANNEL_7;
	   		   break;

	   }
	   if  ( stream == DMA2_CH0 )
	   {
	   		   flag = DMA_INT_TCIFLG0;
	   		   irq = DMA2_STR0_IRQn;
	   		   DMA_CALLback[0].CallBack = f;
	   }
	   else if (stream == DMA2_CH1)
	   {
	   		   flag = DMA_INT_TCIFLG1;
	   		   irq = DMA2_STR1_IRQn;
	   		   DMA_CALLback[1].CallBack = f;
	   }
	   else if  (stream == DMA2_CH2)
	   {
	   		   flag = DMA_INT_TCIFLG2;
	   		   irq = DMA2_STR2_IRQn;
	   		   DMA_CALLback[2].CallBack = f;
	   }
	   else if  (stream == DMA2_CH3)
	   {
	   		   flag = DMA_INT_TCIFLG3;
	   		   irq = DMA2_STR3_IRQn;
	   		   DMA_CALLback[3].CallBack = f;
	   }
	   else if  (stream == DMA2_CH4)
	   {
	   		   flag = DMA_INT_TCIFLG4;
	   		   irq = DMA2_STR4_IRQn;
	   		   DMA_CALLback[4].CallBack = f;
	   }
	   else if  (stream == DMA2_CH5)
	   {
	   		   flag = DMA_INT_TCIFLG5;
	   		   irq = DMA2_STR5_IRQn;
	   		   DMA_CALLback[5].CallBack = f;
	   }
	   else if  (stream == DMA2_CH6)
	   {
	   		   flag = DMA_INT_TCIFLG6;
	   		   irq = DMA2_STR6_IRQn;
	   		   DMA_CALLback[6].CallBack = f;
	   }
	   else
	   {
	   		   flag = DMA_INT_TCIFLG7;
	   		   irq = DMA2_STR7_IRQn;
	   		   DMA_CALLback[7].CallBack = f;
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
	   DMA_Config(stream, &dmaConfig);
	   stream->SCFG_B.EN = ENABLE;  //Enable the specified DMA channel
	   DMA_ClearIntFlag(stream, flag);
	   NVIC_EnableIRQRequest( irq, prior , subprior);
}




void HAL_DMA_TCEnable( DMA_Stram_t chanel)
{
	chanel->SCFG |= (uint32_t)(DMA_INT_TCIFLG & 0x0000001E); //   Enable the specified DMAy channelx interrupts
	chanel->SCFG_B.EN = ENABLE;  //Enable the specified DMA channel
}


void HAL_DMA_SerSource( DMA_Stram_t chanel, uint16_t * data, uint16_t size )
{
	chanel->NDATA = size;         //Configs the number of data units in the channel.
	chanel->M0ADDR = (u32) data;  //Configures the Memory address for the next buffer transfer in double
								  //buffer mode (for dynamic use).
}


uint8_t HAL_DMA_ReadIntFlag(DMA_Stream_T* stream, DMA_INT_FLAG_T flag)
{
    uint32_t tmpreg = 0, enablestatus = 0;

    /* Check if the interrupt enable bit is in the FCTRL or SCFG register */
    if ((flag & 0x60000000) != RESET)
    {
        /* Check the enable bit in FEIEN register */
        enablestatus = stream->FCTRL_B.FEIEN;
    }
    else
    {
        /* Get the interrupt enable position mask in SCFG register */
        tmpreg = ((flag & 0xE000)>> 11) ;

        /* Check the enable bit in SCFG register */
        enablestatus = (stream->SCFG & tmpreg);
    }

    /* Check if the interrupt pending flag is in LINTSTS or HINTSTS */
    if ((flag & 0x10000000) == RESET)
    {
        tmpreg = DMA2->LINTSTS;
    }
    else
    {
        tmpreg = DMA2->HINTSTS;
    }

    tmpreg &= 0x0F7D0F7D;

    if (((tmpreg & flag) != RESET) && (enablestatus != RESET))
    {
        return SET ;
    }
    else
    {
        return RESET ;
    }
}


void DMA2_STR4_IRQHandler( void )
{
	//if ( HAL_DMA_ReadIntFlag(DMA2_Stream4, DMA_INT_TCIFLG4) ==SET)
	{
		DMA2->HIFCLR = ( DMA_INT_TCIFLG4 & 0x0F7D0F7D);  //Clear the DMAy channelx's interrupt pending bits.
		DMA2_Stream4->SCFG &= ~(uint32_t)(DMA_INT_TCIFLG & 0x0000001E); //   Disable the specified DMAy channelx interrupts.
		DMA_CALLback[4].CallBack();
	}
}

void DMA2_STR2_IRQHandler( void )
{
	//if ( HAL_DMA_ReadIntFlag(DMA2_Stream2, DMA_INT_TCIFLG2) ==SET)
	{
		DMA2_Stream2->SCFG &= ~(uint32_t)(DMA_INT_TCIFLG & 0x0000001E); //   Disable the specified DMAy channelx interrupts.
		DMA2->LIFCLR = (DMA_INT_TCIFLG2 & 0x0F7D0F7D);  //Clear the DMAy channelx's interrupt pending bits.
		DMA_CALLback[2].CallBack();
	}
}
void DMA2_STR0_IRQHandler( void )
{
	//if ( HAL_DMA_ReadIntFlag(DMA2_Stream0, DMA_INT_TCIFLG0) ==SET)
	{
		DMA2_Stream0->SCFG &= ~(uint32_t)(DMA_INT_TCIFLG & 0x0000001E); //   Disable the specified DMAy channelx interrupts.
		DMA2->LIFCLR = (DMA_INT_TCIFLG0 & 0x0F7D0F7D);  //Clear the DMAy channelx's interrupt pending bits.
		DMA_CALLback[0].CallBack();
	}
}

#endif
