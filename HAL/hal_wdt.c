/*
 * hal_wdt.c
 *
 *  Created on: 26 邪锌褉. 2024 谐.
 *      Author: i.dymov
 */
#include "hal_wdt.h"
#if MCU == APM32
	#include "apm32f4xx_iwdt.h"
#endif
#if MCU == CH32V2
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
#if MCU == CH32V2
	   IWDG->CTLR = IWDG_WriteAccess_Enable;
	   IWDG->PSCR = IWDG_Prescaler_64;
	   IWDG->RLDR = 4000;
	   IWDG->CTLR = CTLR_KEY_Reload;
	   IWDG->CTLR = CTLR_KEY_Enable;
#endif
#endif
}

void HAL_WDTReset()
{
#ifdef WDT
#if MCU == APM32
	IWDT_Refresh();
#endif
#if MCU == CH32V2
	IWDG->CTLR = CTLR_KEY_Reload;
#endif
#endif
}
/*
 *
 */
void HAL_WDT_Init1s()
{
#ifdef WDT
#if MCU == CH32V2
    IWDG->CTLR = IWDG_WriteAccess_Enable;
	IWDG->PSCR = IWDG_Prescaler_64;
	IWDG->RLDR =  625;
    IWDG->CTLR = CTLR_KEY_Reload;
    IWDG->CTLR = CTLR_KEY_Enable;
#endif
#endif
}

