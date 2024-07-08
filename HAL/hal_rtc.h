/*
 * hal_rtc.h
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_RTC_H_
#define HAL_HAL_RTC_H_

#include "hal_config.h"

void vRTCInit();
void RTC_IRQHandler ( void );
void HAL_RTC_IT_Init(  void (* rtc_it_callback) ( void ), uint8_t prior, uint8_t subprior );


#endif /* HAL_HAL_RTC_H_ */
