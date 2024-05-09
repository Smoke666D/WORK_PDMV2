/*
 * hal_wdt.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_wdt.h"
#if MCU == APM32
	#include "apm32f4xx_iwdt.h"
#endif
#if MCU == CH32
	#include "ch32v20x_iwdg.h"
#endif

void HAL_WDTInit()
{
#ifdef WDT
#if MCU == APM32
	 IWDT_EnableWriteAccess();
	 IWDT_ConfigDivider(IWDT_DIVIDER_16);
	 IWDT_ConfigReload(4000);
	 IWDT_Refresh();
	 IWDT_Enable();
#endif
#if MCU == CH32
	   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	   IWDG_SetPrescaler(IWDG_Prescaler_32);
	   IWDG_SetReload(4000);
	   IWDG_ReloadCounter();
	   IWDG_Enable();
#endif
#endif
}

void HAL_WDTReset()
{
#ifdef WDT
#if MCU == APM32
	IWDT_Refresh();
#endif
#if MCU == CH32
	IWDG_ReloadCounter();
#endif
#endif
}
