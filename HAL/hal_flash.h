/*
 * hal_flash.h
 *
 *  Created on: 15 мая 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_FLASH_H_
#define HAL_HAL_FLASH_H_

#include "hal_config.h"
#include "main.h"

#if MCU == CH32V2
 #include "ch32v20x_flash.h"
#endif

 void HAL_FLASH_WriteByWord( uint8_t * src, uint8_t * dest, uint32_t len);
 void HAL_FLASH_Lock();
 void HAL_FLASH_ErasePage(uint32_t Page_Address);

#endif /* HAL_HAL_FLASH_H_ */
