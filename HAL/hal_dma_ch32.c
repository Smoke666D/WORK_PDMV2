/*
 * hal_dma.c
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_dma.h"

#if MCU == CH32V2
DMA_CFG_t DMA_CALLback[7]   __SECTION(RAM_SECTION_CCMRAM);
    #include <hal_irq.h>
    #include "ch32v20x_dma.h"
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

	    DMA_CALLback[stream].CallBack = f;
	    HAL_DMA_Disable(stream);
	    uint32_t  tmpreg = DMACH[stream]->CFGR & CFGR_CLEAR_Mask;
	    switch (dma_size)
	    {
	        case DMA_BYTE:
	           tmpreg |= DMA_MemoryDataSize_Byte | DMA_PeripheralDataSize_Byte;
	           break;
	       case DMA_HWORD:
	           tmpreg |= DMA_MemoryDataSize_HalfWord | DMA_PeripheralDataSize_HalfWord;
	           break;
	       default:
	           tmpreg |= DMA_MemoryDataSize_Word | DMA_PeripheralDataSize_Word;
	           break;
	    }
	    tmpreg |= direction | DMA_Mode_Normal | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable  |DMA_Priority_Medium | DMA_M2M_Disable;
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
