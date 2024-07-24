/*
 * flash_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */



#include "flash_task.h"
#include "lua_task.h"
#include "hal_flash.h"

static FLASH_LOCK flashLock = FLASH_LOCKED;

static void eFLASHlock ( void )
{
  flashLock = FLASH_LOCKED;
  HAL_FLASH_Lock();
}
FLASH_STATE eFLASHwriteScript ( uint32_t adr, const uint8_t* data, uint32_t length )
{
  u8 size;
  FLASH_STATE       res       = FLASH_OK;
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
    	 if (length == 34 )
    	 {
    		if ((FLASH_STORAGE_ADR + adr) & 0x003)
    	    {
    		    res = HAL_FLASH_WriteWord((uint16_t *)(data ), FLASH_STORAGE_ADR + adr  );
    		    if (res == FLASH_OK)
    			res = HAL_FLASH_WriteBy32Word((uint32_t *)(data+2), FLASH_STORAGE_ADR + adr +2,  32);
    		}
    		else
    		{
    			res = HAL_FLASH_WriteBy32Word((uint32_t *)(data), FLASH_STORAGE_ADR + adr , 32);
    			if (res == FLASH_OK)
    			res = HAL_FLASH_WriteWord((uint16_t *)(data +32 ), FLASH_STORAGE_ADR + adr +32  );
    		}
    	 }
    	 else
    	      res = HAL_FLASH_WriteByByte(data, FLASH_STORAGE_ADR + adr,length );

      }
  if ( res != FLASH_OK)
	  {
	  ( void )eFLASHlock();
	  }
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
  HAL_FLASH_Unlock();

  if (  HAL_FLASH_EraseSector(FLASH_STORAGE_SECTOR )== FMC_COMPLETE)
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



