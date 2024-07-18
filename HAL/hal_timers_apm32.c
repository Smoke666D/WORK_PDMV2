/*
 * hal_timers.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "hal_timers.h"
#include "system_init.h"
//#include "hw_lib_din.h"


static void vTimerInitRCC(TimerName_t TimerName);
static uint32_t getTimerFreq( TimerName_t TimerName );

static TMR_T * timers[TIMERS_COUNT] = { TMR1,TMR2,TMR3,TMR4,TMR5,TMR6,TMR7,TMR8,TMR9,TMR10,TMR11,TMR12,TMR13,TMR14};
static TimerConfif_t config[TIMERS_COUNT]  __SECTION(RAM_SECTION_CCMRAM);

/*
 *  Функция измения частоты работы скнофигурированого таймера на ходу
 */
void vHAL_SetTimerFreq( TimerName_t TimerName, uint32_t freq_in_hz  )
{
	HAL_TiemrEneblae(TimerName);
	timers[TimerName]->PSC_B.PSC =( getTimerFreq(TimerName) / freq_in_hz);
	timers[TimerName]->CEG_B.UEG = TMR_PSC_RELOAD_IMMEDIATE;
	HAL_TiemrDisable(TimerName);
}


void vHW_L_LIB_FreeRunInit( TimerName_t TimerName, uint32_t freq_in_hz  )
{
	uint32_t Freq= getTimerFreq( TimerName );
	vTimerInitRCC(TimerName );
	config[TimerName].Period = 0xFFFF;
	config[TimerName].Div = (Freq /freq_in_hz);
	HW_TIMER_BaseTimerInit(TimerName);
}



void HAL_TiemrEneblae( TimerName_t TimerName )
{
	timers[TimerName]->CTRL1_B.CNTEN = ENABLE;


}

void HAL_TiemrDisable( TimerName_t TimerName )
{
	timers[TimerName]->CTRL1_B.CNTEN = DISABLE;
}

void  HW_TIMER_BaseTimerInit(TimerName_t TimerName  )
{
	TMR_BaseConfig_T TMR_TimeBaseStruct;
	TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
	TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
	TMR_TimeBaseStruct.division = config[TimerName].Div;
	TMR_TimeBaseStruct.period =config[TimerName].Period;
	TMR_ConfigTimeBase(timers[TimerName], &TMR_TimeBaseStruct);
	timers[TimerName]->CTRL1_B.ARPEN = DISABLE;   //Disable peripheral Preload register on AUTORLD.
	timers[TimerName]->SMCTRL_B.SMFSEL = BIT_RESET; // Configures the TMRx internal Clock
}

void HAL_TIMER_PWMTimersInit(TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel)
{
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
	HAL_TiemrEneblae(TimerName);
	TMR_EnablePWMOutputs(timers[TimerName]);

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




uint16_t vHAL_CaptureTimerInteruptCallback(TimerName_t TimerName , uint16_t TimInterupt  )
{
	uint16_t temp_data = 0;
	if ( TMR_ReadIntFlag( timers[TimerName], TimInterupt) == SET )
	{
		temp_data = TMR_ReadCaputer1( timers[TimerName] );
		if ((temp_data) > EDGE)
		{
			TMR_ConfigCounter( timers[TimerName], 0x0000);
		}
		TMR_ClearIntFlag( timers[TimerName], TimInterupt);
	}
	return (temp_data);
}


void HAL_TIMER_SetPWMPulse( TimerName_t TimerName , uint8_t channel, uint32_t pulse )
{
	HAL_TIMER_DisablePWMCH(TimerName , channel );
	if (channel &  TIM_CHANNEL_1) TMR_ConfigCompare1(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_2) TMR_ConfigCompare2(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_3) TMR_ConfigCompare3(timers[TimerName],pulse);
	if (channel &  TIM_CHANNEL_4) TMR_ConfigCompare4(timers[TimerName],pulse);
}



void HAL_TIMER_DisablePWMCH(TimerName_t TimerName , uint8_t channel )
{
    if (channel & TMR_CHANNEL_1)
    {
    	timers[TimerName]->CCEN_B.CC1EN = BIT_RESET;
    }
    if (channel & TMR_CHANNEL_2)
    {
    	timers[TimerName]->CCEN_B.CC2EN = BIT_RESET;
    }
    if (channel & TMR_CHANNEL_3)
    {
    	timers[TimerName]->CCEN_B.CC3EN = BIT_RESET;
    }
    if (channel & TMR_CHANNEL_4)
    {
    	timers[TimerName]->CCEN_B.CC4EN = BIT_RESET;
    }
}

void HAL_TIMER_EnablePWMCH(TimerName_t TimerName , uint8_t channel )
{
	if (channel & TMR_CHANNEL_1)
	{
		timers[TimerName]->CCEN_B.CC1EN = BIT_SET;
	}
	if (channel & TMR_CHANNEL_2)
	{
	    timers[TimerName]->CCEN_B.CC2EN = BIT_SET;
	}
	if (channel & TMR_CHANNEL_3)
	{
	   timers[TimerName]->CCEN_B.CC3EN = BIT_SET;
	}
	if (channel & TMR_CHANNEL_4)
	{
	    timers[TimerName]->CCEN_B.CC4EN = BIT_SET;
	}
}


uint32_t uGetFreeRuningTimer(TimerName_t TimerName )
{
	uint32_t data;
	data = timers[TimerName]->CNT;
	timers[TimerName]->CNT = 0;
	return ( data );
}

/*
 *  Функция иницализаиурет тактирование таймера и возвращает указатель
 *  на handle нужного таймера
 */
static void vTimerInitRCC(TimerName_t TimerName)
{
	u8  RCM_Num = 0;
	u32 RCM_MASK;
	switch (TimerName)
	{
		case TIMER1:
			RCM_Num  = 1;
			RCM_MASK =RCM_APB2_PERIPH_TMR1;
			break;
		case TIMER2:
			RCM_MASK = RCM_APB1_PERIPH_TMR2;
		    break;
		case TIMER3:
			RCM_MASK = RCM_APB1_PERIPH_TMR3;
		    break;
		case TIMER4:
			RCM_MASK =RCM_APB1_PERIPH_TMR4;
			break;
		case TIMER5:
			RCM_MASK =RCM_APB1_PERIPH_TMR5;
		     break;
		case TIMER6:
			RCM_MASK =RCM_APB1_PERIPH_TMR6;
			 break;
		case TIMER7:
			RCM_MASK =RCM_APB1_PERIPH_TMR7;
			 break;
		case TIMER8:
			RCM_Num = 1;
			RCM_MASK =RCM_APB2_PERIPH_TMR8;
			 break;
		case TIMER9:
			RCM_Num = 1;
			RCM_MASK =RCM_APB2_PERIPH_TMR9;
			 break;
		case TIMER10:
			RCM_Num = 1;
			RCM_MASK =RCM_APB2_PERIPH_TMR10;
		     break;
		case TIMER11:
			RCM_Num = 1;
			RCM_MASK =RCM_APB2_PERIPH_TMR11;
		     break;
		case TIMER12:
			RCM_MASK =RCM_APB1_PERIPH_TMR12;
			 break;
		case TIMER13:
			RCM_MASK =RCM_APB1_PERIPH_TMR13;
			 break;
		default:
			RCM_MASK =RCM_APB1_PERIPH_TMR14;
		     break;
	}
	if (RCM_Num == 0)
	{
		RCM->APB1CLKEN |= RCM_MASK;
		RCM->APB1RST |= RCM_MASK;
		RCM->APB1RST &= (uint32_t)~RCM_MASK;
	}
	else
	{
		RCM->APB2CLKEN |=RCM_MASK;
		RCM->APB2RST |=RCM_MASK;
		RCM->APB2RST &= (uint32_t)~RCM_MASK;
	}

}


static uint32_t getTimerFreq( TimerName_t TimerName )
{
	switch (TimerName)
		{
			case TIMER1:
			case TIMER8:
			case TIMER9:
			case TIMER10:
			case TIMER11:
			     return  ( 168000000U );
			default:
				return  ( 84000000U );
		}
	return 0;
}

