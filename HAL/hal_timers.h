/*
 * hal_timers.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_TIMERS_H_
#define HAL_HAL_TIMERS_H_

#include "main.h"
#include "apm32f4xx_tmr.h"
#include "apm32f4xx_rcm.h"

#define EDGE 3

#define TIM_CHANNEL_1  0x01
#define TIM_CHANNEL_2  0x02
#define TIM_CHANNEL_3  0x04
#define TIM_CHANNEL_4  0x08


#define TIM_UPDATE_FLAG TMR_INT_UPDATE
#define TIM_CC1_FLAG   TMR_INT_CC1
#define TIM_CC2_FLAG   TMR_INT_CC2
#define TIM_CC3_FLAG   TMR_INT_CC3
#define TIM_CC4_FLAG   TMR_INT_CC4

#define TIM10_INT  TMR1_UP_TMR10_IRQn
#define TIM9_INT   TMR1_BRK_TMR9_IRQn

typedef enum {
	FREE_RUN_TIMER,
	PWM_TIMER,

} TimerType_t;

typedef enum {
	TIMER1 = 0,
	TIMER2 = 1,
	TIMER3 = 2,
	TIMER4 = 3,
	TIMER5 = 4,
	TIMER6 = 5,
	TIMER7 = 6,
	TIMER8 = 7,
	TIMER9 = 8,
	TIMER10 =9,
	TIMER11 =10,
	TIMER12 = 11,
	TIMER13 = 12,
	TIMER14  =13,
} TimerName_t;


typedef struct
{
	uint16_t Period;
	uint16_t Div;
} TimerConfif_t;

void vHW_L_LIB_FreeRunEneblae( TimerName_t TimerName );
void vHW_L_LIB_FreeRunInit( TimerName_t TimerName, uint32_t freq_in_hz  );
uint32_t uGetFreeRuningTimer(TimerName_t TimerName );
void vHAL_TiemrEneblae( TimerName_t TimerName );
void vHAL_TiemrDisable( TimerName_t TimerName );
void vHAL_EnableTimerIRQ(TimerName_t TimerName,uint8_t priority, uint16_t interupt, uint8_t INT_FLAG );
void vHAL_SetTimerFreq( TimerName_t TimerName, uint32_t freq_in_hz  );
void vHAL_EnablePWMCH(TimerName_t TimerName , uint8_t channel );
void vHW_L_LIB_DisablePWMCH(TimerName_t TimerName , uint8_t channel );
void vHW_L_LIB_PWMTimersInit(TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel);
void vHW_L_LIB_SetPWMPulse( TimerName_t TimerName , uint8_t channel, uint32_t pulse );
void vHAL_InitCaptureIRQTimer( TimerName_t TimerName , uint32_t freq_in_hz, uint32_t Period, uint8_t channel );
uint16_t vHAL_CaptureTimerInteruptCallback(TimerName_t TimerName , uint16_t TimInterupt  );
void vHW_L_LIB_BaseTimerInit(TimerName_t TimerName  );

#endif /* HAL_HAL_TIMERS_H_ */
