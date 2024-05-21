/*
 * drivers_config.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "drivers_config.h"


static TIMER_Type * base_timer;

void vHWLilBaseTimerInit(TIMER_Type * tim )
{
	base_timer  = tim;
#if PLATFORM == CH32V2
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //Таймер управляющий индикацие
    TIM_DeInit(tim );
    TIM_InternalClockConfig(tim  );
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = 1000;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72;
    TIM_TimeBaseInit( tim , &TIM_TimeBaseInitStructure);
    TIM_Cmd(tim  , ENABLE );
#endif
#if PLATFORM == APM32
    TMR_BaseConfig_T baseConfig;
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR13);
    baseConfig.division = 72;
    baseConfig.period = 1000;
    baseConfig.clockDivision = TMR_CLOCK_DIV_1;
    baseConfig.countMode = TMR_COUNTER_MODE_UP;
    TMR_ConfigTimeBase( tim, &baseConfig);
    TMR_EnableUpdate(tim);
#endif
}


uint16_t GetBaseTick()
{
#if PLATFORM == APM32
	return TMR_ReadCounter(base_timer);
#endif
}


uint16_t GetDelay( uint16_t start_delay)
{
    uint16_t cur_tic = GetBaseTick();
    if (cur_tic < start_delay)
    {
        return ((1000-start_delay) + cur_tic);
    }
    else
       return (cur_tic -start_delay);

}
