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


#include "apm32f4xx_fmc.h"
#if MCU == CH32V2
 #include "ch32v20x_flash.h"
#endif

typedef enum
{
  FLASH_OK,
  FLASH_ERROR_ADR,
  FLASH_ERROR_LENGTH,
  FLASH_ERROR_UNLOCK,
  FLASH_ERROR_LOCK,
  FLASH_ERROR_ACCESS,
  FLASH_ERROR_WRITING,
  FLASH_ERROR_WRITING_TIMEOUT,
  FLASH_ERROR_VERIFICATION
} FLASH_STATE;

FMC_STATUS_T  HAL_FLASH_WriteWord( uint16_t * src, uint32_t  address);
FMC_STATUS_T  HAL_FLASH_WriteByByte( uint8_t * src, uint32_t  address, uint32_t len);
FMC_STATUS_T  HAL_FLASH_WriteByWord( uint16_t * src, uint32_t  address, uint32_t len);
FMC_STATUS_T  HAL_FLASH_WriteBy32Word( uint32_t * src, uint32_t  address, uint32_t len);
FMC_STATUS_T  HAL_FLASH_WriteBy64Word( uint64_t * src, uint32_t  address, uint32_t len);
 void HAL_FLASH_Lock();
 void HAL_FLASH_Unlock();
 FMC_STATUS_T HAL_FLASH_ProgrammByte( uint32_t address, uint8_t data);
 FMC_STATUS_T HAL_FLASH_ProgrammWord( uint32_t address, uint16_t data);
 void HAL_FLASH_ErasePage(uint32_t Page_Address);
 FMC_STATUS_T HAL_FLASH_EraseSector(FMC_SECTOR_T sector);

#endif /* HAL_HAL_FLASH_H_ */
