/*
 * flash_task.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_FLASH_TASK_H_
#define DRIVERS_FLASH_TASK_H_

/*----------------------- Includes -------------------------------------*/
#include "main.h"
#include "hal_flash.h"
/*------------------------ Define --------------------------------------*/
#define  FLASH_STORAGE_ADR         0x080E0000UL
#define  FLASH_STORAGE_LENGTH      0x20000U /* byte */
#define  FLASH_STORAGE_SECTOR       FMC_SECTOR_11
#define  FLASH_STORAGE_VOLTAGE     FMC_VOLTAGE_3
#define  FLASH_STORAGE_LENGTH_SIZE 4U
/*------------------------- Macros -------------------------------------*/

/*-------------------------- ENUM --------------------------------------*/


typedef enum
{
  FLASH_LOCKED,
  FLASH_UNLOCKED
} FLASH_LOCK;
/*----------------------- Structures -----------------------------------*/

/*------------------------ Functions -----------------------------------*/
FLASH_STATE eFLASHwriteScript ( uint32_t adr, const uint8_t* data, uint32_t length );
FLASH_STATE eFLASHreadScript ( uint32_t adr, uint8_t* data, uint32_t length );
FLASH_STATE eFLASHstartWriting ( void );
FLASH_STATE eFLASHendWriting ( void );
FLASH_LOCK  eFLASHgetLockState ( void );
const char* uFLASHgetScript ( void );
uint32_t    uFLASHgetLength ( void );
/*----------------------------------------------------------------------*/
void vFLASHTask(void * argument);

#endif /* DRIVERS_FLASH_TASK_H_ */
