/*
 * hal_rtc.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_rtc.h"

#if MCU == APM32
	#include "apm32f4xx.h"
	#include "apm32f4xx_rtc.h"
	#include "apm32f4xx_pmu.h"
	#include "apm32f4xx_rcm.h"
#endif
#if MCU == CH32

#endif

//Callbacl функция для вызова из секундрого прерывания RTC
static void (* func)( void);

void vRTCInit()
{
#if MCU ==  APM32
	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
	PMU_EnableBackupAccess();
	RCM_EnableLSI();
	RTC_Reset();
	RTC_Config_T Struct;
	RCM_EnableLSI();
	while(RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);
	RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
	RCM_EnableRTCCLK();
	RTC_DisableWriteProtection();
	RTC_WaitForSynchro();
	RTC_ConfigStructInit(&Struct);
	RTC_Config(&Struct);
#endif
}

void HAL_RTC_IT_Init(  void (* rtc_it_callback) ( void ))
{
#if MCU == CH32V2
	  NVIC_InitTypeDef      NVIC_InitStructure = {0};
	  uint8_t temp = 0;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR_BackupAccessCmd(ENABLE);
	  /* Is it the first configuration */
	  BKP_DeInit();
	  RCC_LSEConfig(RCC_LSE_ON);
	  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
	  {
	            temp++;
	            Delay_Ms(20);
	  }
	  if(temp >= 250)
	  return;

	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	  RCC_RTCCLKCmd(ENABLE);
	  RTC_WaitForLastTask();
	  RTC_WaitForSynchro();

	  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	  RTC_WaitForLastTask();
	  RTC_EnterConfigMode();
	  RTC_SetPrescaler(32767);
	  RTC_WaitForLastTask();
	  RTC_ExitConfigMode();

	  NVIC_InitStructure.NVIC_IRQChannel =  RTC_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
#endif
	  func = rtc_it_callback;
	  return;
}


void RTC_IRQHandler ( void )
{
#if MCU== CH32V2
    if (RTC_GetITStatus(RTC_FLAG_SEC) == SET)
    {
#endif
        func();
    	// vIncrementSystemCounters();
#if MCU == CH32V2
        RTC_ClearITPendingBit(RTC_FLAG_SEC);
    }
#endif
    return;

}
