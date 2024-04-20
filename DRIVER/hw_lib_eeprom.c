/*
 * hw_lib_eeprom.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "hw_lib_eeprom.h"
#include "string.h"
#include "system_init.h"
//#include "FreeRTOS.h"
//#include "event_groups.h"




static EEPOROM  Dev __SECTION(RAM_SECTION_CCMRAM);
static void vSetAddr(EEPROM_ADRESS_TYPE addr );

static uint8_t sector_buffer[SECTOR_SIZE + ADDRESS_DATA] __SECTION(RAM_SECTION_CCMRAM);




EEPOROM * xGetEEPROM()
{
	return (&Dev);
}



EERPOM_ERROR_CODE_t eEEPROMWr(  EEPROM_ADRESS_TYPE addr, uint8_t * data, EEPROM_ADRESS_TYPE len )
{
   EERPOM_ERROR_CODE_t res = EEPROM_NOT_VALIDE_ADRESS;
   if ((  addr+ len  <= EEPROM_SIZE ) && (len!=0))
   {
     if (len == sizeof(uint8_t))
     {
         sector_buffer[0] =  (addr >> 8) & 0xFF ;
         sector_buffer[1] =  addr & 0xFF ;
         sector_buffer[ADDRESS_DATA] = *data;
         res =  Dev.I2C_Master_Transmit_func( Device_ADD | GET_ADDR_MSB( addr) , (uint8_t *) sector_buffer, ADDRESS_DATA + sizeof(uint8_t) , EEPROM_TIME_OUT )  ;
     }
     else
     {
         EEPROM_ADRESS_TYPE cur_len;
         EEPROM_ADRESS_TYPE byte_to_send = len;
         EEPROM_ADRESS_TYPE offset = 0;
         EEPROM_ADRESS_TYPE cur_addr = addr;
         uint32_t timeout = 0;
         while  (byte_to_send > 0)
         {
             cur_len = SECTOR_SIZE - ( cur_addr % SECTOR_SIZE );
             if ( cur_len > byte_to_send )  cur_len = byte_to_send;
             memcpy( &sector_buffer[ADDRESS_DATA], &data[offset], cur_len );
             sector_buffer[0] =  (cur_addr >> 8) & 0xFF ;
             sector_buffer[1] =  cur_addr & 0xFF ;

             while (timeout < 5000)
             {
                uint32_t start_tick = GetBaseTick();
                res = Dev.I2C_Master_Transmit_func( Device_ADD | GET_ADDR_MSB( cur_addr) ,(uint8_t *) sector_buffer,  cur_len + ADDRESS_DATA , EEPROM_TIME_OUT );
                if  (res != EEPROM_OK)
                {
                     timeout = GetDelay(start_tick);
                 }
                 else break;
             }
             offset         = offset  + cur_len;
             byte_to_send   = byte_to_send - cur_len;
             cur_addr       = cur_addr  + cur_len;
         }
       }
   }

   return ( res );
}

EERPOM_ERROR_CODE_t eEEPROMRd(  EEPROM_ADRESS_TYPE addr, uint8_t * data,   EEPROM_ADRESS_TYPE len )
{
    EERPOM_ERROR_CODE_t res = EEPROM_NOT_VALIDE_ADRESS;
    if ( (addr +len)  <= EEPROM_SIZE)
    {
         vSetAddr( addr);
         res = Dev.I2C_Master_Transmit_func( Device_ADD | GET_ADDR_MSB( addr ), Dev.ADDR, ADDRESS_DATA ,EEPROM_TIME_OUT );
         if (res ==EEPROM_OK)
         {
                return (  Dev.I2C_Master_Recive_func(  Device_ADD, data, len ,EEPROM_TIME_OUT) );
         }
     }
     Dev.SetReady_func();
     return ( res );
}

static void vSetAddr(EEPROM_ADRESS_TYPE addr )
{
      Dev.ADDR[0] = (addr >> 8) & 0xFF ;
      Dev.ADDR[1] =  addr & 0xFF;
}

uint8_t bReadEEPROM( EEPROM_ADRESS_TYPE addr )
{
    u8 data;
    eEEPROMRd(addr, &data,1);
    return (data);
}



