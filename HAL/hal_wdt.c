/*
 * hal_wdt.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_wdt.h"
#include "apm32f4xx_iwdt.h"


void HAL_WDTInit()
{
#ifdef WDT
	 IWDT_EnableWriteAccess();
	 IWDT_ConfigDivider(IWDT_DIVIDER_16);
	 IWDT_ConfigReload(4000);
	 IWDT_Refresh();
	 IWDT_Enable();
#endif
}

void HAL_WDTReset()
{
#ifdef WDT
	IWDT_Refresh();
#endif
}
