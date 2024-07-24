/*
 * hal_wdt.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_wdt.h"
#if MCU == APM32
	#include "apm32f4xx_iwdt.h"



void HAL_WDTInit()
{
#ifdef WDT


	 IWDT->KEY_B.KEY = IWDT_WRITEACCESS_ENABLE; //Enable the IWDT write access
	 IWDT->PSC = IWDT_DIVIDER_16;   //Set IWDT frequency divider values
	 IWDT->CNTRLD = 4000;   //Set IWDT count reload values
	 IWDT->KEY = IWDT_KEYWORD_RELOAD;  //  Reload the IWDT counter with value
	 IWDT->KEY = IWDT_KEYWORD_ENABLE;  //Enable IWDT


#endif
}

void HAL_WDTReset()
{
#ifdef WDT
	IWDT->KEY = IWDT_KEYWORD_RELOAD;  //  Reload the IWDT counter with value
#endif
}
/*
 *
 */

#endif
