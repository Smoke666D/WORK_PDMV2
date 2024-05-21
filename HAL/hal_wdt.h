/*
 * hal_wdt.h
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_WDT_H_
#define HAL_HAL_WDT_H_

#include "main.h"
#include "hal_config.h"


typedef enum
{
  ms0_1,
  ms1,
  ms10,
  ms100,
  s1,
  s10,
  s24
} HAL_WDT_Timeouts_t;

void HAL_WDTInit();
void HAL_WDTReset();

void HAL_WDT_InitInit( HAL_WDT_Timeouts_t );
void HAL_WDT_Init1s();



#endif /* HAL_HAL_WDT_H_ */
