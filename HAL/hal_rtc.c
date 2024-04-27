/*
 * hal_rtc.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_rtc.h"
#include "apm32f4xx.h"
#include "apm32f4xx_rtc.h"
#include "apm32f4xx_pmu.h"
#include "apm32f4xx_rcm.h"

void vRTCInit()
{
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
}
