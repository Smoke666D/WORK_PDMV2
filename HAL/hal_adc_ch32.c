/*
 * hal_adc_ch32.c
 *
 *  Created on: 24 懈褞谢. 2024 谐.
 *      Author: i.dymov
 */

#include "hal_adc.h"


#if MCU == CH32V2

#include "ch32v20x_adc.h"

#include "hal_irq.h"
ADC_t adcs;
static s16 Calibrattion_Val = 0;
static const  uint8_t ADC_chennel_ref[]={  ADC_Channel_0,  ADC_Channel_1,  ADC_Channel_2, ADC_Channel_3, ADC_Channel_4,  ADC_Channel_5,  ADC_Channel_6,  ADC_Channel_7,  ADC_Channel_8,  ADC_Channel_9,  ADC_Channel_10,
        ADC_Channel_11, ADC_Channel_12, ADC_Channel_13, ADC_Channel_14, ADC_Channel_15,  ADC_Channel_16,  ADC_Channel_17} ;


/* ADC SQx ma   /* ADC ADON mask */
#define CTLR2_ADON_Set                   ((uint32_t)0x00000001)
#define CTLR2_ADON_Reset                 ((uint32_t)0xFFFFFFFE)
/* ADC DMA mask */
#define CTLR2_DMA_Set                    ((uint32_t)0x00000100)
#define CTLR2_DMA_Reset                  ((uint32_t)0xFFFFFEFF)
/* ADC Software start mask */
#define CTLR2_EXTTRIG_SWSTART_Set        ((uint32_t)0x00500000)
#define CTLR2_EXTTRIG_SWSTART_Reset      ((uint32_t)0xFFAFFFFF)
/* ADC RSTCAL mask */
#define CTLR2_RSTCAL_Set                 ((uint32_t)0x00000008)
/* ADC CAL mask */
#define CTLR2_CAL_Set                    ((uint32_t)0x00000004)
/* ADC EXTTRIG mask */
#define CTLR2_EXTTRIG_Set                ((uint32_t)0x00100000)
#define CTLR2_EXTTRIG_Reset              ((uint32_t)0xFFEFFFFF)sk */
#define RSQR3_SQ_Set                     ((uint32_t)0x0000001F)
#define RSQR2_SQ_Set                     ((uint32_t)0x0000001F)
#define RSQR1_SQ_Set                     ((uint32_t)0x0000001F)
/* ADC SMPx mask */
#define SAMPTR1_SMP_Set                  ((uint32_t)0x00000007)
#define SAMPTR2_SMP_Set                  ((uint32_t)0x00000007)
/* CTLR1 register Mask */
#define CTLR1_CLEAR_Mask                 ((uint32_t)0xE0F0FEFF)
/* CTLR2 register Mask */
#define CTLR2_CLEAR_Mask                 ((uint32_t)0xFFF1F7FD)
/* RSQR1 register Mask */
#define RSQR1_CLEAR_Mask                 ((uint32_t)0xFF0FFFFF)
#define CFGR0_ADCPRE_Reset_Mask     ((uint32_t)0xFFFF3FFF)
#if ADC_1_2_ENABLE == 1
void ADC1_2_IRQHandler(void) __attribute__((interrupt()));
#endif
u16 Get_ConversionVal(s16 val)
{
    if((val+Calibrattion_Val)<0|| val==0) return 0;
    if((Calibrattion_Val+val)>4095||val==4095) return 4095;
    return (val+Calibrattion_Val);
}

static ADC_TypeDef* ADCS[]={ADC1,ADC2};
static inline void ADC_ENABLE( ADC_NUMBER_t adc )  {  ADCS[adc]->CTLR2 |= CTLR2_ADON_Set; }
static inline void ADC_DISABLE (ADC_NUMBER_t adc  ) { ADCS[adc]->CTLR2 &= CTLR2_ADON_Reset;}
static inline void ADC_BUFFER_ENABLE ( ADC_NUMBER_t adc ) {   ADCS[adc]->CTLR1 |= (1 << 26);  }
static inline void ADC_BUFFER_DISABLE ( ADC_NUMBER_t adc ) {  ADCS[adc]->CTLR1 &= ~(1 << 26);  }
static inline void ADC_DMA_ENABLE ( ADC_NUMBER_t adc ) {  ADCS[adc]->CTLR2 |= CTLR2_DMA_Set; }
static inline void ADC_DMA_DISABLE ( ADC_NUMBER_t adc ) {  ADCS[adc]->CTLR2  &= CTLR2_DMA_Reset; }
static inline void ADC_RESET_CAL( ADC_NUMBER_t adc  )  { ADCS[adc]->CTLR2 |= CTLR2_RSTCAL_Set; }
static inline void ADC_START_CAL ( ADC_NUMBER_t adc  )     {ADCS[adc]->CTLR2 |= CTLR2_CAL_Set; }
static inline void ADC_EXT_TRIG_ENABLE ( ADC_NUMBER_t adc )  {ADCS[adc]->CTLR2 |= CTLR2_EXTTRIG_Set;}


void HAL_ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0;

#if ADC_CHANNEL_NUMBER_10_16 == 1
    if(ADC_Channel > ADC_Channel_9)
    {
        tmpreg1 = ADCx->SAMPTR1;
        tmpreg2 = SAMPTR1_SMP_Set << (3 * (ADC_Channel - 10));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR1 = tmpreg1;
    }
    else
#endif
    {
        tmpreg1 = ADCx->SAMPTR2;
        tmpreg2 = SAMPTR2_SMP_Set << (3 * ADC_Channel);
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
        tmpreg1 |= tmpreg2;
        ADCx->SAMPTR2 = tmpreg1;
    }
#if ADC_CHANNELRANK_0_6 == 1
    if(Rank < 7)
    {
        tmpreg1 = ADCx->RSQR3;
        tmpreg2 = RSQR3_SQ_Set << (5 * (Rank - 1));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR3 = tmpreg1;
    }
#endif
#if ADC_CHANNELRANK_7_12 == 1
    else if(Rank < 13)
    {
        tmpreg1 = ADCx->RSQR2;
        tmpreg2 = RSQR2_SQ_Set << (5 * (Rank - 7));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR2 = tmpreg1;
    }
#endif
#if ADC_CHANNELRANK_13_16 == 1
    else
    {
        tmpreg1 = ADCx->RSQR1;
        tmpreg2 = RSQR1_SQ_Set << (5 * (Rank - 13));
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
        tmpreg1 |= tmpreg2;
        ADCx->RSQR1 = tmpreg1;
    }
#endif
}

void HAL_ADC_ContiniusScanCinvertionDMA( ADC_NUMBER_t adc, uint8_t channel_count, uint8_t *  channel_nmber)
 {
#if ADC_1_ENABLE == 1
    if ( adc == ADC_1)
    {
        RCC->APB2PCENR |= RCC_APB2Periph_ADC1;
        RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
        RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;

    }
#endif
#if ADC_2_ENABLE == 1
    else
    {
        RCC->APB2PCENR |= RCC_APB2Periph_ADC2;
        RCC->APB2PRSTR |= RCC_APB2Periph_ADC2;
        RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC2;

    }
#endif
     uint32_t tmpreg = 0;
     tmpreg = RCC->CFGR0;
     tmpreg &= CFGR0_ADCPRE_Reset_Mask;
     tmpreg |= RCC_PCLK2_Div8;
     RCC->CFGR0 = tmpreg;

     ADCS[adc]->CTLR1 = (uint32_t)(ADC_Mode_Independent | (uint32_t)ADC_OutputBuffer_Disable |(uint32_t)ADC_Pga_1 | ((uint32_t)ENABLE<< 8));
     ADCS[adc]->CTLR2 = (uint32_t)( ADC_DataAlign_Right | ADC_ExternalTrigConv_None | ((uint32_t)ENABLE<< 1));
     ADCS[adc]->RSQR1 = ((uint32_t)(ADC_CHANNEL-1)) << 20;;
     for (u8 i=0; i< (channel_count) ;i++)
     {
         HAL_ADC_RegularChannelConfig(ADCS[adc],ADC_chennel_ref[channel_nmber[ i  ]], i + 1,  ADC_SampleTime_239Cycles5 );
     }
     ADC_EXT_TRIG_ENABLE( adc);
     ADC_DMA_ENABLE (adc);
     ADC_ENABLE(adc);
     ADC_BUFFER_DISABLE(adc);
     ADC_RESET_CAL(adc);
     while(ADC_GetResetCalibrationStatus(ADCS[adc]));
     ADC_START_CAL(adc);
     while(ADC_GetCalibrationStatus(ADCS[adc]));
     Calibrattion_Val = Get_CalibrationValue(ADCS[adc]);
     ADC_BUFFER_ENABLE(adc);
 }


 void HAL_ADC_StartDMA( DMA_Stram_t chanel, uint16_t size)
 {
    HAL_DMA_SetCounter(chanel, size);
    ADC_DMA_ENABLE (ADC_1);
    ADCS[ADC_1]->STATR = ~(uint32_t)(ADC_IT_EOC >> 8);
    HAL_DMA_ITENABLE( chanel, DMA_IT_TC );
    HAL_DMA_Enable(chanel );
    ADC_ENABLE(ADC_1);
    ADCS[ADC_1]->CTLR2 |= CTLR2_EXTTRIG_SWSTART_Set;
}


#if ADC_1_2_ENABLE == 1

void ADC1_2_IRQHandler(void)
{
    if(ADC_GetITStatus( ADC1, ADC_IT_AWD))
    {
        ADC_ClearITPendingBit( ADC1, ADC_IT_AWD);
        adcs.awdt_callback();
    }
}
#endif

#endif

