/*
 * hal_timers.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "hal_timers.h"
#include "hal_irq.h"



static void vTimerInitRCC(TimerName_t TimerName);
static uint32_t getTimerFreq( TimerName_t TimerName );

#if MCU == APM32
static TMR_T * timers[TIMERS_COUNT] = { TMR1,TMR2,TMR3,TMR4,TMR5,TMR6,TMR7,TMR8,TMR9,TMR10,TMR11,TMR12,TMR13,TMR14};
#endif
#if MCU == CH32V2
static TIM_TypeDef * timers[TIMERS_COUNT] = { TIM1,TIM2,TIM3,TIM4};
#if TIM1_UP_ENABLE == 1
void TIM1_UP_IRQHandler(void) __attribute__((interrupt()));
#endif
#if TIM2_UP_ENABLE == 1
void TIM2_IRQHandler(void) __attribute__((interrupt()));
#endif
#if TIM3_UP_ENABLE == 1
void TIM3_IRQHandler(void) __attribute__((interrupt()));
#endif
#if TIM4_UP_ENABLE == 1
void TIM4_IRQHandler(void) __attribute__((interrupt()));
#endif

#endif
static TimerConfif_t config[TIMERS_COUNT];



/*
 *  Функция измения частоты работы скнофигурированого таймера на ходу
 */
void vHAL_SetTimerFreq( TimerName_t TimerName, uint32_t freq_in_hz  )
{
#if MCU == APM32
	TMR_Disable(timers[TimerName]);
	TMR_ConfigPrescaler(timers[TimerName],( getTimerFreq(TimerName) / freq_in_hz) ,TMR_PSC_RELOAD_IMMEDIATE);
	TMR_Enable(timers[TimerName]);
#endif
}


void vHW_L_LIB_FreeRunInit( TimerName_t TimerName, uint32_t freq_in_hz  )
{

	vTimerInitRCC(TimerName );
#if MCU == APM32
	uint32_t Freq= getTimerFreq( TimerName );
	config[TimerName].Period = 0xFFFF;
	config[TimerName].Div = (Freq /freq_in_hz);
#endif
	HW_TIMER_BaseTimerInit(TimerName);

}

void HAL_TIMER_InitIt( TimerName_t TimerName, uint32_t freq_in_hz, uint32_t Period, void (*f)() ,uint8_t prior, uint8_t subprior )
{
#if MCU == CH32V2
	uint32_t Freq = getTimerFreq( TimerName );
	vTimerInitRCC(TimerName) ;
	config[TimerName].Period = Period;
    config[TimerName].Div = (Freq /freq_in_hz);
    config[TimerName].callback_function = f;
    HW_TIMER_BaseTimerInit(TimerName);
    timers[TimerName]->INTFR = (uint16_t)~TIM_IT_Update;
    timers[TimerName]->DMAINTENR |=  TIM_IT_Update;
	uint8_t irq;
	switch (TimerName )
	{
	    case TIMER1:
	        irq = TIM1_UP_IRQn;
	        break;
		case TIMER2:
			irq = TIM2_IRQn;
			break;
		case TIMER3:
			irq = TIM3_IRQn;
			break;
		default:
			irq = TIM4_IRQn;
			break;
	}
	PFIC_IRQ_ENABLE_PG1(irq,prior,subprior);
#endif
}

uint16_t bufdata = 0;
u8 over = 0;
#if MCU == CH32V2

void static TimerUPIrq( TimerName_t TimerName )
{
    if   (SET == TIM_GetITStatus(timers[TimerName], TIM_IT_Update) )
    {
        config[TimerName].callback_function();
        timers[TimerName]->INTFR = (uint16_t)~TIM_IT_Update;
    }
}

#if TIM1_UP_ENABLE == 1
void  TIM1_UP_IRQHandler(void)
{
    TimerUPIrq(TIMER1);
}
#endif
#if TIM2_UP_ENABLE == 1
void  TIM2_IRQHandler(void)
{
    TimerUPIrq(TIMER2);
}
#endif
#if TIM3_UP_ENABLE == 1
void  TIM3_IRQHandler(void)
{
    TimerUPIrq(TIMER3);
}
#endif
#if TIM4_UP_ENABLE == 1
void  TIM4_IRQHandler(void)
{
    TimerUPIrq(TIMER4);
}
#endif


#endif

void HAL_TiemrEneblae( TimerName_t TimerName )
{
#if MCU == APM32
	TMR_Enable(timers[TimerName]);
#endif
#if MCU == CH32V2
	timers[TimerName]->CTLR1 |= TIM_CEN;

#endif
}

void HAL_TiemrDisable( TimerName_t TimerName )
{

#if MCU == APM32
	TMR_Disable(timers[TimerName]);
#endif
#if MCU == CH32V2
	timers[TimerName]->CTLR1 &= (uint16_t)(~((uint16_t)TIM_CEN));

#endif
}

u32 HAL_GetTimerCounterRegAdres(TimerName_t TimerName , uint8_t ch )
{
#if MCU == CH32V2
	switch (ch)
	{
		case TIM_CHANNEL_1:
			return (timers[TimerName]->CH1CVR);
		case TIM_CHANNEL_2:
			return (timers[TimerName]->CH2CVR);
		case TIM_CHANNEL_3:
			return (timers[TimerName]->CH3CVR);
		default:
			return (timers[TimerName]->CH4CVR);

	}
#endif
	return 0;
}

void  HW_TIMER_BaseTimerInit(TimerName_t TimerName  )
{
#if MCU == APM32
	TMR_BaseConfig_T TMR_TimeBaseStruct;
	TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
	TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
	TMR_TimeBaseStruct.division = config[TimerName].Div;
	TMR_TimeBaseStruct.period =config[TimerName].Period;
	TMR_ConfigTimeBase(timers[TimerName], &TMR_TimeBaseStruct);
	TMR_DisableAutoReload(timers[TimerName]);
	TMR_ConfigInternalClock(timers[TimerName]);
#endif
#if MCU == CH32V2
	  timers[TimerName]->SMCFGR &= (uint16_t)(~((uint16_t)TIM_SMS)); //Тактирование от внутренней шины
	  uint16_t tmpcr1 = 0;
	  tmpcr1 = timers[TimerName]->CTLR1;
	  tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
	  tmpcr1 |= (uint32_t)TIM_CounterMode_Up;
	  tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
	  tmpcr1 |= (uint32_t)config[TimerName].ClockDiv;
	  timers[TimerName]->CTLR1 = tmpcr1;
	  timers[TimerName]->ATRLR = config[TimerName].Period;
	  timers[TimerName]->PSC = config[TimerName].Div;
	  if((TimerName == TIMER1 ))  timers[TimerName]->RPTCR = 0x0000;
	  timers[TimerName]->SWEVGR = TIM_PSCReloadMode_Immediate;
#endif
}

void HAL_TIMER_PWMTimersInit(TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel)
{
#if MCU == APM32
	TMR_OCConfig_T OCcongigStruct;
	uint32_t Freq = getTimerFreq( TimerName );
	vTimerInitRCC(TimerName) ;
	config[TimerName].Period = Period;
	config[TimerName].Div = (Freq /freq_in_hz);
	HW_TIMER_BaseTimerInit(TimerName);
	OCcongigStruct.mode = TMR_OC_MODE_PWM1;
	OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;
	OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
	OCcongigStruct.idleState = TMR_OC_IDLE_STATE_RESET;
	OCcongigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;
	OCcongigStruct.nPolarity = TMR_OC_NPOLARITY_HIGH;
	OCcongigStruct.outputNState = TMR_OC_NSTATE_DISABLE;
	OCcongigStruct.pulse = Period;// + 1;
	if (channel &  TIM_CHANNEL_1)
	{ TMR_ConfigOC1(timers[TimerName], &OCcongigStruct);
	  TMR_ConfigOC1Fast(timers[TimerName],TMR_OC_FAST_ENABLE);
	  TMR_ConfigOC1Preload(timers[TimerName], TMR_OC_PRELOAD_ENABLE);
	  TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_1); }
	if (channel &  TIM_CHANNEL_2)
	{ TMR_ConfigOC2(timers[TimerName], &OCcongigStruct);
	  TMR_ConfigOC2Fast(timers[TimerName],TMR_OC_FAST_ENABLE);
	  TMR_ConfigOC2Preload(timers[TimerName], TMR_OC_PRELOAD_ENABLE);
	  TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_2);}
	if (channel &  TIM_CHANNEL_3)
	{ TMR_ConfigOC3(timers[TimerName], &OCcongigStruct);
	  TMR_ConfigOC3Fast(timers[TimerName],TMR_OC_FAST_ENABLE);
	  TMR_ConfigOC3Preload(timers[TimerName], TMR_OC_PRELOAD_ENABLE);
	  TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_3); }
	if (channel &  TIM_CHANNEL_4)
	{ TMR_ConfigOC4(timers[TimerName], &OCcongigStruct);
	  TMR_ConfigOC4Fast(timers[TimerName],TMR_OC_FAST_ENABLE);
	  TMR_ConfigOC4Preload(timers[TimerName], TMR_OC_PRELOAD_ENABLE);
	  TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_4); }
	TMR_EnableAutoReload(timers[TimerName]);
	TMR_Enable(timers[TimerName]);
	TMR_EnablePWMOutputs(timers[TimerName]);
#endif

#if MCU ==  CH32V2
	 TIM_OCInitTypeDef TIM_OCInitStructure={0};
	 uint32_t Freq = getTimerFreq( TimerName );
	 vTimerInitRCC(TimerName) ;
	 config[TimerName].Period = Period;
	 config[TimerName].Div = (Freq /freq_in_hz);
	 config[TimerName].ClockDiv = 0;
	 HW_TIMER_BaseTimerInit(TimerName);

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = Period;
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 if (channel &  TIM_CHANNEL_1)
	 {
		 TIM_OC1Init( timers[TimerName], &TIM_OCInitStructure );
		 TIM_OC1PreloadConfig( timers[TimerName], TIM_OCPreload_Disable );
	 }
	 if (channel &  TIM_CHANNEL_2)
	 {
	     TIM_OC2Init( timers[TimerName], &TIM_OCInitStructure );
		 TIM_OC2PreloadConfig( timers[TimerName], TIM_OCPreload_Disable );
	 }
	 if (channel &  TIM_CHANNEL_3)
	 {
		TIM_OC3Init( timers[TimerName], &TIM_OCInitStructure );
	    TIM_OC3PreloadConfig( timers[TimerName], TIM_OCPreload_Disable );
	 }
	 if (channel &  TIM_CHANNEL_4)
	 {
		 TIM_OC4Init( timers[TimerName], &TIM_OCInitStructure );
		 TIM_OC4PreloadConfig( timers[TimerName], TIM_OCPreload_Disable );
	 }
	 timers[TimerName]->CTLR1 |= TIM_ARPE;
	 timers[TimerName]->BDTR |= TIM_MOE;

#endif
}


void vHAL_EnableTimerIRQ(TimerName_t TimerName,uint8_t priority, uint16_t interupt, uint8_t INT_FLAG )
{
#if MCU == APM32
	NVIC_EnableIRQRequest((IRQn_Type)interupt, priority, 0);
	TMR_EnableInterrupt(timers[TimerName], INT_FLAG);
#endif
}

IRQn_Type getTimerIRQ(TimerName_t TimerName )
{
#if MCU == APM32
	switch (TimerName)
	{
		case TIMER9:
			return (TMR1_BRK_TMR9_IRQn);
		case TIMER10:
			return (TMR1_UP_TMR10_IRQn);
		default:
			return 0;
		    break;
	}
#endif
}

void HAL_InitCaptureIRQTimer( TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period,   uint8_t channel )
{
#if MCU == APM32
	TMR_ICConfig_T ICConfig;
	TMR_BaseConfig_T baseConfig;
	uint32_t Freq = getTimerFreq( TimerName );
	baseConfig.clockDivision = TMR_CLOCK_DIV_4;
	baseConfig.countMode = TMR_COUNTER_MODE_UP;
	baseConfig.division =  (Freq /freq_in_hz);// 16800;
	baseConfig.period =  Period;
	baseConfig.repetitionCounter = 0;
	TMR_ConfigTimeBase(timers[TimerName] , &baseConfig);
	TMR_ConfigInternalClock(timers[TimerName]);
    if (channel &  TIM_CHANNEL_1) { ICConfig.channel = TMR_CHANNEL_1;  TMR_EnableInterrupt(timers[TimerName], TIM_CC1_FLAG);}
    if (channel &  TIM_CHANNEL_2) { ICConfig.channel = TMR_CHANNEL_2;  TMR_EnableInterrupt(timers[TimerName], TIM_CC2_FLAG);}
    if (channel &  TIM_CHANNEL_3) { ICConfig.channel = TMR_CHANNEL_3;  TMR_EnableInterrupt(timers[TimerName], TIM_CC3_FLAG);}
    if (channel &  TIM_CHANNEL_4) { ICConfig.channel = TMR_CHANNEL_4;  TMR_EnableInterrupt(timers[TimerName], TIM_CC4_FLAG);}
	ICConfig.polarity = TMR_IC_POLARITY_BOTHEDGE;
	ICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
	ICConfig.prescaler = TMR_IC_PSC_1;
	ICConfig.filter = 0x00;
	TMR_ConfigPWM(timers[TimerName], &ICConfig);
	NVIC_EnableIRQRequest(getTimerIRQ(TimerName), 5, 0);
	HAL_TiemrEneblae( TimerName );
#endif
}



#if MCU == CH32V2

static void TI2_Config(TimerName_t TimerName, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;

    timers[TimerName]->CCER &= (uint16_t) ~((uint16_t)TIM_CC2E);
    tmpccmr1 = timers[TimerName]->CHCTLR1;
    tmpccer = timers[TimerName]->CCER;
    tmp = (uint16_t)(TIM_ICPolarity << 4);
    tmpccmr1 &= (uint16_t)(((uint16_t) ~((uint16_t)TIM_CC2S)) & ((uint16_t) ~((uint16_t)TIM_IC2F)));
    tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
    tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);
    tmpccer &= (uint16_t) ~((uint16_t)(TIM_CC2P));
    tmpccer |= (uint16_t)(tmp | (uint16_t)TIM_CC2E);
    timers[TimerName]->CHCTLR1 = tmpccmr1;
    timers[TimerName]->CCER = tmpccer;
}

static void TI4_Config(TimerName_t TimerName, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
    uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
    timers[TimerName]->CCER &= (uint16_t) ~((uint16_t)TIM_CC4E);
    tmpccmr2 = timers[TimerName]->CHCTLR2;
    tmpccer = timers[TimerName]->CCER;
    tmp = (uint16_t)(TIM_ICPolarity << 12);
    tmpccmr2 &= (uint16_t)((uint16_t)(~(uint16_t)TIM_CC4S) & ((uint16_t) ~((uint16_t)TIM_IC4F)));
    tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
    tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);
    tmpccer &= (uint16_t) ~((uint16_t)(TIM_CC4P));
    tmpccer |= (uint16_t)(tmp | (uint16_t)TIM_CC4E);
    timers[TimerName]->CHCTLR2 = tmpccmr2;
    timers[TimerName]->CCER = tmpccer;
}

#endif

void HAL_TimeInitCaptureDMA( TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel )
{
    uint32_t Freq = getTimerFreq( TimerName );
#if MCU == CH32V2
     vTimerInitRCC(TimerName) ;
     config[TimerName].Period = Period;
     config[TimerName].Div = (Freq /freq_in_hz) ;
     config[TimerName].ClockDiv = 0;
     HW_TIMER_BaseTimerInit(TimerName);
     if ( channel == TIM_CHANNEL_2 )
     {
         TI2_Config(TimerName, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI, 4);
         timers[TimerName]->CHCTLR1 &= (uint16_t) ~((uint16_t)TIM_IC2PSC);   // Устанавилваем предделитель
         timers[TimerName]->CHCTLR1 |= (uint16_t)(TIM_ICPSC_DIV1 << 8);
         timers[TimerName]->DMAINTENR |= ( TIM_IT_CC2 | TIM_DMA_CC2 ) ;      //Рзрешаем прерывание и работу по DMA
     }
     if ( channel == TIM_CHANNEL_4 )
     {
          TI4_Config(TimerName, TIM_ICPolarity_Rising,  TIM_ICSelection_DirectTI,  4);
          timers[TimerName]->CHCTLR2 &= (uint16_t) ~((uint16_t)TIM_IC4PSC); // Устанавилваем предделитель
          timers[TimerName]->CHCTLR2 |= (uint16_t)(TIM_ICPSC_DIV1 << 8);
          timers[TimerName]->DMAINTENR |= ( TIM_IT_CC4 | TIM_DMA_CC4 ) ;    //Рзрешаем прерывание и работу по DMA
      }
#endif

}



uint16_t vHAL_CaptureTimerInteruptCallback(TimerName_t TimerName , uint16_t TimInterupt  )
{
	uint16_t temp_data = 0;
#if MCU == APM32
	if ( TMR_ReadIntFlag( timers[TimerName], TimInterupt) == SET )
	{
		temp_data = TMR_ReadCaputer1( timers[TimerName] );
		if ((temp_data) > EDGE)
		{
			TMR_ConfigCounter( timers[TimerName], 0x0000);
		}
		TMR_ClearIntFlag( timers[TimerName], TimInterupt);
	}
#endif
	return (temp_data);
}


void HAL_TIMER_SetPWMPulse( TimerName_t TimerName , uint8_t channel, uint32_t pulse )
{
	HAL_TIMER_DisablePWMCH(TimerName , channel );
#if MCU == APM32
	if (channel &  TIM_CHANNEL_1) TMR_ConfigCompare1(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_2) TMR_ConfigCompare2(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_3) TMR_ConfigCompare3(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_4) TMR_ConfigCompare4(timers[TimerName],pulse);
#endif
#if MCU == CH32V2
	 TIM_OCInitTypeDef TIM_OCInitStructure={0};
	 timers[TimerName]->BDTR &= (uint16_t)(~((uint16_t)TIM_MOE)); //Выключаем выхода ШИМ
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = (uint16_t) pulse ;
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 if (channel &  TIM_CHANNEL_1) TIM_OC1Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_2) TIM_OC2Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_3) TIM_OC3Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_4) TIM_OC4Init( timers[TimerName], &TIM_OCInitStructure );
	 timers[TimerName]->BDTR |= TIM_MOE;   //Включем выхода ШИМ
#endif

}



void HAL_TIMER_DisablePWMCH(TimerName_t TimerName , uint8_t channel )
{
#if MCU == APM32
	if (channel &  TIM_CHANNEL_1) TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_1);
	if (channel &  TIM_CHANNEL_2) TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_2);
	if (channel &  TIM_CHANNEL_3) TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_3);
	if (channel &  TIM_CHANNEL_4) TMR_DisableCCxChannel(timers[TimerName],TMR_CHANNEL_4);
#endif
#if MCU == CH32V2
	  timers[TimerName]->BDTR &= (uint16_t)(~((uint16_t)TIM_MOE));
#endif
}

void HAL_TIMER_EnablePWMCH(TimerName_t TimerName , uint8_t channel )
{
#if MCU == APM32
	if (channel &  TIM_CHANNEL_1) TMR_EnableCCxChannel(timers[TimerName],TMR_CHANNEL_1);
	if (channel &  TIM_CHANNEL_2) TMR_EnableCCxChannel(timers[TimerName],TMR_CHANNEL_2);
	if (channel &  TIM_CHANNEL_3) TMR_EnableCCxChannel(timers[TimerName],TMR_CHANNEL_3);
	if (channel &  TIM_CHANNEL_4) TMR_EnableCCxChannel(timers[TimerName],TMR_CHANNEL_4);
#endif
#if MCU == CH32V2
	timers[TimerName]->BDTR |= TIM_MOE;

#endif
}


uint32_t uGetFreeRuningTimer(TimerName_t TimerName )
{
	uint32_t data;

#if MCU == APM32
	data = TMR_ReadCounter(timers[TimerName]);
	TMR_ConfigCounter(timers[TimerName],0);
#endif
	return ( data );
}

/*
 *  Функция иницализаиурет тактирование таймера и возвращает указатель
 *  на handle нужного таймера
 */
static void vTimerInitRCC(TimerName_t TimerName)
{
    switch (TimerName)
    {
        case TIMER1:
#if MCU == APM32
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
#endif
#if MCU == CH32V2
            RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
            RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;
            RCC->APB2PCENR |= RCC_APB2Periph_TIM1;
#endif
            break;
        case TIMER2:
#if MCU == APM32
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);
#endif
#if MCU == CH32V2
            RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
            RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;
            RCC->APB1PCENR |= RCC_APB1Periph_TIM2;
#endif
            break;
        case TIMER3:
#if MCU == APM32
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
#endif
#if MCU == CH32V2
            RCC->APB1PRSTR |= RCC_APB1Periph_TIM3;
            RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM3;
            RCC->APB1PCENR |= RCC_APB1Periph_TIM3;
#endif
            break;
#if MCU == APM32
        case TIMER4:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR4);
            break;
        case TIMER5:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);
        case TIMER6:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR6);
             break;
        case TIMER7:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR7);
             break;
        case TIMER8:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR8);
             break;
        case TIMER9:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR9);
             break;
        case TIMER10:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR10);
             break;
        case TIMER11:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR11);
             break;
        case TIMER12:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR12);
             break;
        case TIMER13:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR13);
             break;
        case TIMER14:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);

             break;
#endif
        default:
#if MCU == CH32V2
            RCC->APB1PRSTR |= RCC_APB1Periph_TIM4;
            RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM4;
            RCC->APB1PCENR |= RCC_APB1Periph_TIM4;
#endif
            break;

    }
}


static uint32_t getTimerFreq( TimerName_t TimerName )
{
#if MCU == CH32V2
                return ( 72000000U);
#endif
#if MCU == APM32
    switch (TimerName)
        {
            case TIMER1:
                return  ( 168000000U );
            case TIMER2:
                return  ( 84000000U );
            case TIMER3:
                return  ( 84000000U );
            case TIMER4:
                return  ( 84000000U );
            case TIMER5:
                return  ( 84000000U );
            case TIMER6:
                return  ( 84000000U );
            case TIMER7:
                return  ( 84000000U );
            case TIMER8:
                return  ( 168000000U );
            case TIMER9:
                return  ( 168000000U );
            case TIMER10:
                return  ( 168000000U );
            case TIMER11:
                 return  ( 168000000U );
            case TIMER12:
                return  ( 84000000U );
            case TIMER13:
                return  ( 84000000U );
            case TIMER14:
                return  ( 84000000U );

        }
    return 0;
#endif
}



