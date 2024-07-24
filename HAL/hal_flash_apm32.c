/*
 * hal_flash_apm32.c
 *
 *  Created on: 23 июл. 2024 г.
 *      Author: i.dymov
 */
#include "hal_flash.h"


#if MCU == APM32

#define  FLASH_STORAGE_VOLTAGE     FMC_VOLTAGE_3
#include "apm32f4xx_fmc.h"


 void HAL_FLASH_Lock()
 {
	 FMC->CTRL |= FMC_CTRL_LOCK;
 }

 void HAL_FLASH_Unlock()
 {
	 if((FMC->CTRL & FMC_CTRL_LOCK) != RESET)
	     {
	         FMC->KEY = FMC_KEY1;
	         FMC->KEY = FMC_KEY2;
	     }
 }
 FMC_STATUS_T HAL_FLASH_ProgrammByte( uint32_t address, uint8_t data)
 {


	     FMC_STATUS_T status = FMC_WaitForLastOperation();

	    if (status == FMC_COMPLETE)
	    {
	        FMC->CTRL &= 0xFFFFFCFF;
	        FMC->CTRL |= FMC_PSIZE_BYTE;
	        FMC->CTRL |= FMC_CTRL_PG;
	        *(__IO uint8_t*)address = data;
	        status = FMC_WaitForLastOperation();
	        FMC->CTRL &= (~FMC_CTRL_PG);
	    }
	    return status;
 }

 FMC_STATUS_T HAL_FLASH_ProgrammWord( uint32_t address, uint16_t data)
 {


	    FMC_STATUS_T status = FMC_WaitForLastOperation();

	    if (status == FMC_COMPLETE)
	    {
	        FMC->CTRL &= 0xFFFFFCFF;
	        FMC->CTRL |= FMC_PSIZE_HALF_WORD;
	        FMC->CTRL |= FMC_CTRL_PG;
	        *(__IO uint16_t*)address = data;
	        status = FMC_WaitForLastOperation();
	        FMC->CTRL &= (~FMC_CTRL_PG);
	    }
	    return status;
 }

 FMC_STATUS_T HAL_FLASH_Programm32Word( uint32_t address, uint32_t data)
 {


	    FMC_STATUS_T status = FMC_WaitForLastOperation();

	    if (status == FMC_COMPLETE)
	    {
	    	FMC->CTRL &= 0xFFFFFCFF;
	    	        FMC->CTRL |= FMC_PSIZE_WORD;
	    	        FMC->CTRL |= FMC_CTRL_PG;
	    	        *(__IO uint32_t*)address = data;
	    	        status = FMC_WaitForLastOperation();
	    	        FMC->CTRL &= (~FMC_CTRL_PG);
	    }
	    return status;
 }




 FMC_STATUS_T HAL_FLASH_EraseSector(FMC_SECTOR_T sector)
{
	 FMC->STS  = (FMC_FLAG_ERROP | FMC_FLAG_ERRWRP | FMC_FLAG_ERRPGA | FMC_FLAG_ERRPGP | FMC_FLAG_ERRPGS  );
     FMC_STATUS_T status = FMC_COMPLETE;
     status = FMC_WaitForLastOperation();
     if (status == FMC_COMPLETE)
     {
         FMC->CTRL &= 0xFFFFFCFF;
         FMC->CTRL |= FMC_PSIZE_DOUBLE_WORD;
         FMC->CTRL &= 0xFFFFFF07;
         FMC->CTRL |= FMC_CTRL_SERS | sector;
         FMC->CTRL |= FMC_CTRL_START;
         status = FMC_WaitForLastOperation();
         FMC->CTRL &= (~FMC_CTRL_SERS);
         FMC->CTRL &= 0xFFFFFF07;
     }
     return status;
}
 FMC_STATUS_T   HAL_FLASH_WriteByByte( uint8_t * src, uint32_t  address, uint32_t len)
 {
	 uint32_t curentAdr = address;
	 for ( uint32_t i=0U; i<len; i++ )
	         {
	           curentAdr = address + ( i * sizeof( uint8_t ) );
	           if ( HAL_FLASH_ProgrammByte(  curentAdr, src[i] ) != FMC_COMPLETE )
	           {
	             return (FLASH_ERROR_WRITING);

	           }
	           else if ( *( uint8_t* )( curentAdr ) != src[i] )
	           {
	             return ( FLASH_ERROR_VERIFICATION);

	           }
	         }
	 return (FLASH_OK);
 }


 FMC_STATUS_T   HAL_FLASH_WriteWord( uint16_t * src, uint32_t  address)
  {

	  if ( HAL_FLASH_ProgrammWord(  address, * src  ) != FMC_COMPLETE )
	  {
	 	  return (FLASH_ERROR_WRITING);
	  }
	  else if ( *( uint16_t* )(  address ) != *src )
	  {
	 	  return ( FLASH_ERROR_VERIFICATION);
	  }
	  return (FLASH_OK);
  }


 FMC_STATUS_T   HAL_FLASH_WriteByWord( uint16_t * src, uint32_t  address, uint32_t len)
  {
	  uint32_t curentAdr = address;
	  for ( uint32_t i=0U; i<len/2; i++ )
	 {
	 	     curentAdr = address + ( i * sizeof( uint16_t ) );
	 	     if ( HAL_FLASH_ProgrammWord( curentAdr, src[i]  ) != FMC_COMPLETE )
	 	           {
	 	             return (FLASH_ERROR_WRITING);

	 	           }
	 	           else if ( *( uint16_t* )( curentAdr ) != src[i] )
	 	           {
	 	             return ( FLASH_ERROR_VERIFICATION );

	 	           }
	 	         }
	 	 return (FLASH_OK);
  }


 FMC_STATUS_T   HAL_FLASH_WriteBy32Word( uint32_t * src, uint32_t  address, uint32_t len)
  {
	   uint32_t curentAdr = address;
	   FMC_STATUS_T status = FMC_WaitForLastOperation();
	   if (status != FMC_COMPLETE) return (FLASH_ERROR_WRITING);
	   FMC->CTRL &= 0xFFFFFCFF;
	   FMC->CTRL |= FMC_PSIZE_WORD;

	   for ( uint32_t i=0U; i<len/4; i++ )
	   {
		 	  curentAdr = address + ( i * sizeof( uint32_t ) );

		 	  FMC_STATUS_T status = FMC_WaitForLastOperation();

		 	  if (status == FMC_COMPLETE)
		 	  {
		 		   FMC->CTRL |= FMC_CTRL_PG;
		 		   *(__IO uint32_t*)curentAdr = src[i] ;
		 		   status = FMC_WaitForLastOperation();
		 		   FMC->CTRL &= (~FMC_CTRL_PG);
		 		}
		 	  if ( status != FMC_COMPLETE )
		 	  {
		 	     return (FLASH_ERROR_WRITING);
		 	  }
		 	  else if ( *( uint32_t* )( curentAdr ) != src[i])
		 	  {
		 	     return ( FLASH_ERROR_VERIFICATION);
		 	  }
		}
		return (FLASH_OK);
  }


#endif
