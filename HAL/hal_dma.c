/*
 * hal_dma.c
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_dma.h"
#include "hal_irq.h"

#if MCU == APM32
	#include "apm32f4xx_dma.h"
	#include "apm32f4xx_rcm.h"
	#include "apm32f4xx_adc.h"
#endif

DMA_CFG_t DMA_CALLback[7]   __SECTION(RAM_SECTION_CCMRAM);

#if MCU == CH32V2
    #include "ch32v20x_dma.h"
	#include "ch32v20x_adc.h"
	#include "ch32v20x_rcc.h"
/* DMA registers Masks */
#define CFGR_CLEAR_Mask          ((uint32_t)0xFFFF800F)
DMA_Channel_TypeDef * DMACH[]={DMA1_Channel1,DMA1_Channel2,DMA1_Channel3,DMA1_Channel4,DMA1_Channel5,DMA1_Channel6,DMA1_Channel7};
#if DMA1_CH1_ENABLE == 1
void     DMA1_Channel1_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH2_ENABLE == 1
void     DMA1_Channel2_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH3_ENABLE == 1
void     DMA1_Channel3_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH4_ENABLE == 1
void     DMA1_Channel4_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH5_ENABLE == 1
void     DMA1_Channel5_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH6_ENABLE == 1
void     DMA1_Channel6_IRQHandler(void)   __attribute__((interrupt()));
#endif
#if DMA1_CH7_ENABLE == 1
void     DMA1_Channel7_IRQHandler(void)   __attribute__((interrupt()));
#endif

void HAL_DMAInitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, uint32_t memadr,  uint8_t prior, uint8_t subprior, void (*f)(void))
{
	   /* Enable DMA clock */
	          RCC->AHBPCENR |= RCC_AHBPeriph_DMA1;

	          u32 DMA_DIR = ( direction== MTOP ) ?DMA_DIR_PeripheralDST : DMA_DIR_PeripheralSRC;
	          u32 DMA_MemoryDataSize;
	          u32 DMA_PeripheralDataSize;
	          switch (dma_size)
	          {
	              case DMA_BYTE:
	                  DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	                  DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	                  break;
	              case DMA_HWORD:
	                  DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	                  DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	                  break;
	              default:
	                  DMA_MemoryDataSize = DMA_MemoryDataSize_Word ;
	                  DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word ;
	                  break;
	          }

	          DMA_CALLback[stream].CallBack = f;
	          HAL_DMA_Disable(stream);
	          uint32_t tmpreg = 0;
	          tmpreg = DMACH[stream]->CFGR;
	          tmpreg &= CFGR_CLEAR_Mask;
	          tmpreg |= DMA_DIR | DMA_Mode_Normal | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable |
	                       DMA_PeripheralDataSize | DMA_MemoryDataSize |DMA_Priority_Medium | DMA_M2M_Disable;

	          DMACH[stream]->CFGR = tmpreg;
	          DMACH[stream]->CNTR = 0;
	          DMACH[stream]->PADDR = paddr;
	          DMACH[stream]->MADDR = memadr;
	          DMACH[stream]->CFGR |= DMA_IT_TC;
	          PFIC_IRQ_ENABLE_PG1(DMA1_Channel1_IRQn  + stream ,prior,subprior);

}

/*
 * Включить выбрвнный канал DMA
 */
void HAL_DMA_Enable(DMA_Stram_t stream  )
{
    DMACH[stream]->CFGR |= DMA_CFGR1_EN;
}
/*
 * Выключить выбранный канал DMA
 */

void HAL_DMA_Disable(DMA_Stram_t stream  )
{
    DMACH[stream]->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
}

void HAL_DMA_SetCounter( DMA_Stram_t stream, uint32_t counter )
{
    DMACH[stream]->CNTR = counter;
}

void HAL_DMA_ITENABLE( DMA_Stram_t stream, uint32_t it )
{
    DMACH[stream]->CFGR |=it;
}

void HAL_DMA_SetCouterAndEnable(DMA_Stram_t stream, uint32_t counter )
{
    DMACH[stream]->CNTR = counter;
    DMACH[stream]->CFGR |= DMA_CFGR1_EN;
}



static void DMA_IRQ( u8 ch)
{
    if ( DMA_GetITStatus(DMA1_IT_TC1<<(4*ch))==SET )
    {
         DMA_CALLback[ch].CallBack();
         DMA_ClearITPendingBit(DMA1_IT_GL1<<(4*ch)); //Сбрасываем флаг
    }
    return;

}

#if DMA1_CH1_ENABLE == 1
void DMA1_Channel1_IRQHandler(void)
{
    DMA_IRQ(0);
    return;
}
#endif
#if DMA1_CH2_ENABLE == 1
void DMA1_Channel2_IRQHandler(void)
{
    DMA_IRQ(1);
    return;
}
#endif
#if DMA1_CH3_ENABLE == 1
void DMA1_Channel3_IRQHandler(void)
{
    DMA_IRQ(2);
    return;
}
#endif
#if DMA1_CH4_ENABLE == 1
void  DMA1_Channel4_IRQHandler(void)
{
    DMA_IRQ(3);
    return;
}
#endif
#if DMA1_CH5_ENABLE == 1
void  DMA1_Channel5_IRQHandler(void)
{
    DMA_IRQ(4);
    return;
}
#endif
#if DMA1_CH6_ENABLE == 1
void  DMA1_Channel6_IRQHandler(void)
{
    DMA_IRQ(5);
    return;
}
#endif
#if DMA1_CH7_ENABLE == 1
void DMA1_Channel7_IRQHandler(void)
{
    DMA_IRQ(6);
    return;
}
#endif


#endif

#if MCU == APM32




void HAL_DMAInitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, uint32_t memadr, DMA_CHANNEL_t channel, uint8_t prior, uint8_t subprior, void (*f)(void))
{

       DMA_Config_T dmaConfig;
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
       dmaConfig.memoryInc         = DMA_MEMORY_INC_ENABLE;
       dmaConfig.peripheralInc     = DMA_PERIPHERAL_INC_DISABLE;
       dmaConfig.loopMode          = DMA_MODE_NORMAL;
       dmaConfig.priority          = DMA_PRIORITY_LOW;
       dmaConfig.fifoMode          = DMA_FIFOMODE_DISABLE;
       dmaConfig.fifoThreshold     = DMA_FIFOTHRESHOLD_FULL;
       dmaConfig.peripheralBurst   = DMA_PERIPHERALBURST_SINGLE;
       dmaConfig.memoryBurst       = DMA_MEMORYBURST_SINGLE;
       dmaConfig.peripheralBaseAddr = paddr;
       dmaConfig.channel           = ch;
       DMA_Config(stream, &dmaConfig);
       DMA_Enable(stream);
       DMA_ClearIntFlag(stream, flag);
       NVIC_EnableIRQRequest( irq, prior , subprior);

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
#endif
