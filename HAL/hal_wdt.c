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
#if MCU == CH32V2
	IWDG_ReloadCounter();
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
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
    IWDG_SetReload( 625);
    IWDG_ReloadCounter();
    IWDG_Enable();
#endif
#endif
}

/*
 *
 */
void HAL_WDT_InitInit( HAL_WDT_Timeouts_t timeouts)
{
uint8_t prescaler;
uint16_t reload;
#ifdef WDT
#if MCU == CH32V2
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	switch (timeouts)
	{
		case ms0_1:
			prescaler = IWDG_Prescaler_4;
			reload    = 1;
			break;
		case ms1:
			prescaler = IWDG_Prescaler_8;
			reload    = 5;
			break;
		case ms10:
			prescaler = IWDG_Prescaler_16;
			reload    = 25;
			break;
		case ms100:
			prescaler = IWDG_Prescaler_32;
			reload    = 125;
			break;
		case s1:
			prescaler = IWDG_Prescaler_64;
			reload    = 625;
			break;
		case s10:
			prescaler = IWDG_Prescaler_128;
			reload    = 3125;
			break;
		case s24:
			prescaler = IWDG_Prescaler_256;
			reload    = 3907;
		default:
			break;
	}
	IWDG_SetPrescaler(prescaler);
    IWDG_SetReload(reload);
    IWDG_ReloadCounter();
    IWDG_Enable();
#endif
#endif
}
