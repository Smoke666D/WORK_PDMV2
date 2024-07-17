/*
 * flash_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */



#include "flash_task.h"
//#include "system.h"
#include "lua_task.h"
#include "apm32f4xx_fmc.h"

static FLASH_LOCK flashLock = FLASH_LOCKED;

static void eFLASHlock ( void )
{
  flashLock = FLASH_LOCKED;
  FMC->CTRL |= FMC_CTRL_LOCK; //FLASH LOCK

}


FMC_STATUS_T HAL_FMC_ReadStatus(void)
{
    if ((FMC->STS & FMC_FLAG_BUSY) == FMC_FLAG_BUSY)
    {
        return ( FMC_BUSY);
    }
    if ((FMC->STS & (FMC_FLAG_ERRWRP | FMC_FLAG_ERROP | 0xE0) ) != RESET)
    {
        return ( FMC_ERROR_OPERATION );
    }
    return (FMC_COMPLETE);
}


FMC_STATUS_T HAL_FMC_WaitForLastOperation(void)
{
    __IO FMC_STATUS_T status = FMC_COMPLETE;

    status = HAL_FMC_ReadStatus();

    while (status == FMC_BUSY)
    {
        status = HAL_FMC_ReadStatus();
    }

    return status;
}

FMC_STATUS_T HAL_FMC_ProgramByte(uint32_t address, uint8_t data)
{
    FMC_STATUS_T status = FMC_COMPLETE;
    status = HAL_FMC_WaitForLastOperation();

    if (status == FMC_COMPLETE)
    {
        FMC->CTRL &= 0xFFFFFCFF;
        FMC->CTRL |= FMC_PSIZE_BYTE;
        FMC->CTRL |= FMC_CTRL_PG;
        *(__IO uint8_t*)address = data;
        status = HAL_FMC_WaitForLastOperation();
        FMC->CTRL &= (~FMC_CTRL_PG);
    }

    return status;
}


FLASH_STATE eFLASHwriteScript ( uint32_t adr, const uint8_t* data, uint32_t length )
{
  FLASH_STATE       res       = FLASH_OK;
  uint32_t          curentAdr = FLASH_STORAGE_ADR + adr;
  if ( flashLock != FLASH_UNLOCKED )
  {
	  res = FLASH_ERROR_ACCESS;
  }
  else
    if ( adr >= FLASH_STORAGE_LENGTH )
    {
    	res = FLASH_ERROR_ADR;
    }
    else
      if ( ( length + adr ) >= FLASH_STORAGE_LENGTH )
      {
    	  res = FLASH_ERROR_LENGTH;
      }
      else
      {
        for ( uint32_t i=0U; i<length; i++ )
        {
          curentAdr = FLASH_STORAGE_ADR + adr + ( i * sizeof( uint8_t ) );
          if ( HAL_FMC_ProgramByte(  curentAdr, data[i] ) != FMC_COMPLETE )
          {
            res = FLASH_ERROR_WRITING;
            break;
          }
          else if ( *( uint8_t* )( curentAdr ) != data[i] )
          {
            res = FLASH_ERROR_VERIFICATION;
            break;
          }
        }
      }
  if ( res != FLASH_OK) ( void )eFLASHlock();
  return res;
}
FLASH_STATE eFLASHreadScript ( uint32_t adr, uint8_t* data, uint32_t length )
{
  FLASH_STATE res = FLASH_OK;
  if ( adr < FLASH_STORAGE_LENGTH )
  {
    if ( ( adr + length ) < FLASH_STORAGE_LENGTH )
    {
      for ( uint32_t i=0U; i<length; i++ )
      {
        data[i] = *( uint8_t* )( FLASH_STORAGE_ADR + adr + ( i * sizeof( uint8_t ) ) );
      }
    }
    else
    {
      res = FLASH_ERROR_LENGTH;
    }
  }
  else
  {
    res = FLASH_ERROR_ADR;
  }
  return res;
}
FLASH_STATE eFLASHstartWriting ( void )
{
  FLASH_STATE res = FLASH_OK;
  vLUAstopPDM();
  FMC_Unlock();
  FMC_ClearStatusFlag(FMC_FLAG_ERROP | FMC_FLAG_ERRWRP | FMC_FLAG_ERRPGA | FMC_FLAG_ERRPGP | FMC_FLAG_ERRPGS  );
  if ( FMC_EraseSector( FLASH_STORAGE_SECTOR, FLASH_STORAGE_VOLTAGE )== FMC_COMPLETE)
  {
    flashLock = FLASH_UNLOCKED;
  }
  else
  {
    res = FLASH_ERROR_UNLOCK;
  }
  return res;
}
FLASH_STATE eFLASHendWriting ( void )
{
   eFLASHlock();
   vLUArestartPDM();
  return (FLASH_OK);
}
FLASH_LOCK  eFLASHgetLockState ( void )
{
  return flashLock;
}
const char* uFLASHgetScript ( void )
{
  return ( const char* )( FLASH_STORAGE_ADR + FLASH_STORAGE_LENGTH_SIZE );
}
uint32_t uFLASHgetLength ( void )
{
  return *( uint32_t* )( FLASH_STORAGE_ADR );
}



