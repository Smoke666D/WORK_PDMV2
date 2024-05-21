/*
 * hal_timers.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "hal_timers.h"
//#include "hw_lib_din.h"


static void vTimerInitRCC(TimerName_t TimerName);
static uint32_t getTimerFreq( TimerName_t TimerName );

#if MCU == APM32
static TMR_T * timers[TIMERS_COUNT] = { TMR1,TMR2,TMR3,TMR4,TMR5,TMR6,TMR7,TMR8,TMR9,TMR10,TMR11,TMR12,TMR13,TMR14};
#endif
#if MCU == CH32V2
static TIM_TypeDef * timers[TIMERS_COUNT] = { TIM1,TIM2,TIM3,TIM4,TIM5};
void TIM2_IRQHandler(void) __attribute__((interrupt()));
void TIM3_IRQHandler(void) __attribute__((interrupt()));
void TIM4_IRQHandler(void) __attribute__((interrupt()));
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
	uint32_t Freq= getTimerFreq( TimerName );

	vTimerInitRCC(TimerName );
#if MCU == APM32
	config[TimerName].Period = 0xFFFF;
	config[TimerName].Div = (Freq /freq_in_hz);
#endif
	HW_TIMER_BaseTimerInit(TimerName);

}

void HAL_TIMER_InitIt( TimerName_t TimerName, uint32_t freq_in_hz, uint32_t Period, void (*f)() )
{
#if MCU == CH32V2
    NVIC_InitTypeDef      NVIC_InitStructure = {0};
	uint32_t Freq = getTimerFreq( TimerName );
	vTimerInitRCC(TimerName) ;
	config[TimerName].Period = Period;
    config[TimerName].Div = (Freq /freq_in_hz);
    config[TimerName].callback_function = f;
    HW_TIMER_BaseTimerInit(TimerName);
	TIM_ClearITPendingBit(timers[TimerName], TIM_IT_Update);
	TIM_ITConfig(timers[TimerName], TIM_IT_Update,ENABLE);
	uint8_t irq;
	switch (TimerName )
	{

		case TIMER2:
			irq = TIM2_IRQn;
			break;
		case TIMER3:
			irq = TIM3_IRQn;
			break;
		case TIMER4:
			irq = TIM4_IRQn;
			break;

	}
    NVIC_InitStructure.NVIC_IRQChannel =  irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}

#if MCU == CH32V2


void  TIM4_IRQHandler(void)
{


    if   (SET == TIM_GetITStatus(TIM4, TIM_IT_Update) )
    {
        config[3].callback_function();
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    }
}


void  TIM3_IRQHandler(void)
{


    if   (SET == TIM_GetITStatus(TIM3, TIM_IT_Update) )
    {
        config[2].callback_function();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    }
}

void  TIM2_IRQHandler(void)
{


    if   (SET == TIM_GetITStatus(TIM2, TIM_IT_Update) )
    {
        config[1].callback_function();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    }
}

#endif

void HAL_TiemrEneblae( TimerName_t TimerName )
{
#if MCU == APM32
	TMR_Enable(timers[TimerName]);
#endif
#if MCU == CH32V2
	 TIM_Cmd(timers[TimerName], ENABLE);
#endif
}

void HAL_TiemrDisable( TimerName_t TimerName )
{

#if MCU == APM32
	TMR_Disable(timers[TimerName]);
#endif
#if MCU == CH32V2
	 TIM_Cmd(timers[TimerName], DISABLE);
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
		case TIM_CHANNEL_4:
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

	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	  TIM_DeInit(timers[TimerName]);
	  TIM_InternalClockConfig(timers[TimerName]);
	  TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	  TIM_TimeBaseInitStructure.TIM_Period = config[TimerName].Period;
	  TIM_TimeBaseInitStructure.TIM_Prescaler = config[TimerName].Div;
	  TIM_TimeBaseInit( timers[TimerName], &TIM_TimeBaseInitStructure);

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
     TIM_DeInit(timers[TimerName]);
	 TIM_InternalClockConfig(timers[TimerName]);
	 vTimerInitRCC(TimerName) ;
	 config[TimerName].Period = Period;
	 config[TimerName].Div = (Freq /freq_in_hz);
	 HW_TIMER_BaseTimerInit(TimerName);

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
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
	 TIM_ARRPreloadConfig( timers[TimerName], ENABLE );
	 TIM_CtrlPWMOutputs(timers[TimerName], ENABLE );
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

void HAL_InitCaptureIRQTimer( TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel )
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

void HAL_InitCaptureDMATimer( TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel )
{
	uint32_t Freq = getTimerFreq( TimerName );
#if MCU == CH32V2

	 TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	 TIM_ICInitTypeDef       TIM_ICInitStructure;
	 TIM_DeInit(timers[TimerName]);
	 TIM_InternalClockConfig(timers[TimerName]);
	 TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	 TIM_TimeBaseInitStructure.TIM_Period = CC_PERIOD;
	 TIM_TimeBaseInitStructure.TIM_Prescaler = (Freq /freq_in_hz);
	 TIM_TimeBaseInit( timers[TimerName] , &TIM_TimeBaseInitStructure);
	 if (channel &  TIM_CHANNEL_1) { TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  TIM_ITConfig(timers[TimerName], TIM_IT_CC1 , ENABLE);}
	 if (channel &  TIM_CHANNEL_2) { TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  TIM_ITConfig(timers[TimerName], TIM_IT_CC2 , ENABLE);}
	 if (channel &  TIM_CHANNEL_3) { TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  TIM_ITConfig(timers[TimerName], TIM_IT_CC3 , ENABLE);}
	 if (channel &  TIM_CHANNEL_4) { TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  TIM_ITConfig(timers[TimerName], TIM_IT_CC4 , ENABLE);}
	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	 TIM_ICInitStructure.TIM_ICFilter = 0x00;
	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	 TIM_PWMIConfig(timers[TimerName], &TIM_ICInitStructure);
	 TIM_SelectInputTrigger(timers[TimerName],TIM_TS_TI2FP2);
	 TIM_SelectSlaveMode(timers[TimerName], TIM_SlaveMode_Reset);
	 TIM_SelectMasterSlaveMode(timers[TimerName], TIM_MasterSlaveMode_Enable);
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
	 TIM_CtrlPWMOutputs(timers[TimerName], DISABLE );
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = (uint16_t) pulse ;
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 if (channel &  TIM_CHANNEL_1) TIM_OC1Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_2) TIM_OC2Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_3) TIM_OC3Init( timers[TimerName], &TIM_OCInitStructure );
	 if (channel &  TIM_CHANNEL_4) TIM_OC4Init( timers[TimerName], &TIM_OCInitStructure );
	TIM_CtrlPWMOutputs(timers[TimerName], ENABLE );
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
		TIM_CtrlPWMOutputs(timers[TimerName], DISABLE);
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
		TIM_CtrlPWMOutputs(timers[TimerName], ENABLE);
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
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
#endif
			break;
		case TIMER2:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);
#endif
#if MCU == CH32V2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
#endif
		    break;
		case TIMER3:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
#endif
#if MCU == CH32V2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#endif
		    break;
		case TIMER4:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR4);
#endif
#if MCU == CH32V2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
#endif
			break;
		case TIMER5:
#if MCU == APM32
		RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);
#endif
#if MCU == CH32V2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
#endif
		     break;
		case TIMER6:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR6);
#endif
#if MCU == CH32V2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
#endif
			 break;
		case TIMER7:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR7);
#endif
#if MCU == CH32V2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
#endif
			 break;
		case TIMER8:
#if MCU == APM32
			RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR8);
#endif
			 break;
		case TIMER9:
#if MCU == APM32
			RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR9);
#endif
			 break;
		case TIMER10:
#if MCU == APM32
			RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR10);
#endif
		     break;
		case TIMER11:
#if MCU == APM32
			RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR11);
			#endif
		     break;
		case TIMER12:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR12);
			#endif
			 break;
		case TIMER13:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR13);
			#endif
			 break;
		case TIMER14:
#if MCU == APM32
			RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);
#endif
		     break;
		default:
			break;

	}
}


static uint32_t getTimerFreq( TimerName_t TimerName )
{
	switch (TimerName)
		{
			case TIMER1:
#if MCU == APM32
				return  ( 168000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER2:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER3:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER4:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER5:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER6:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
			case TIMER7:
#if MCU == APM32
				return  ( 84000000U );
#endif
#if MCU == CH32V2
				return ( 72000000U);
#endif
#if MCU == APM32
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
#endif
		}
	return 0;
}

