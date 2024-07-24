/*
 * hal_flash.c
 *
 *  Created on: 15 мая 2024 г.
 *      Author: i.dymov
 */


#include "hal_flash.h"

#if MCU == CH32V2

 void HAL_FLASH_WriteByWord( uint8_t * src, uint8_t * dest, uint32_t len)
{

	 for (uint32_t i=0U; i<len; i+=4U )
	   {
	 	if ( ( uint32_t )( dest + i ) > FLASH_SIZE )
	 	{
	       if (  FLASH_ProgramWord( ( uint32_t )( dest + i ), *( uint32_t* )( src + i ) ) ==  FLASH_COMPLETE)
	       {
	         if ( *( uint32_t* )( src + i ) != *( uint32_t* )( dest + i ) )
	         {
	           break;
	         }
	         else
	         {

	         }
	       }
	       else
	       {

	         break;
	       }
	 	}
	   }
	   return;


}

 void HAL_FLASH_Lock()
 {

	 FLASH_Lock();

 }
/*
 *  Функция стирания заднной страницы
 *  Page_Address  - адрес начала страницы памяти
 */
 void HAL_FLASH_ErasePage(uint32_t Page_Address)
 {

	 FLASH_Unlock();
	 FLASH_ErasePage(Page_Address);

 }
#endif
