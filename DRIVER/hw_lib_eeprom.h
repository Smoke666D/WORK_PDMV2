/*
 * hw_lib_eeprom.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HW_LIB_HW_LIB_EEPROM_H_
#define HW_LIB_HW_LIB_EEPROM_H_


#include "main.h"
#include "drivers_config.h"
#include "hal_i2c.h"

#define EEPROM_ADRESS_TYPE uint16_t
#define EEPROM_MAX_ADRRES 0x7FF

#if  MCU == APM32
#include "apm32f4xx_i2c.h"
#define I2C_T I2C_T
#define  DIR_TRANSMIT     I2C_DIRECTION_TX
#define  DIR_RECIEVE      I2C_DIRECTION_RX

#endif
#if  MCU == CH32V2

#define  DIR_TRANSMIT I2C_Direction_Transmitter
#define  DIR_RECIEVE  I2C_Direction_Receiver

#endif


#define I2C_ACK_FAIL    0x0001
#define I2C_STOP        0x0002
#define I2C_READY       0x0004
#define EEPROM_DATA_READY  0x0008
#define EEPROM_RX_DATA  0x0010

#define SECTOR_SIZE     16U
#define Device_ADD 0x00A0
#define EEPROM_TIME_OUT 100U
#define EEPROM_SIZE (2U *1024U)

#define ADDRESS_SIZE_BYTES  2
#define ADDRESS_DATA        (ADDRESS_SIZE_BYTES - 1)

/*   алгоритм получания данных для старшего байта адреса  */
#define BYTE_SHIFT          8U
#define MSB_SHIFT           0x07
#define MSB_MASK            0x0E
#define GET_ADDR_MSB( ADDR) (( ADDR >>MSB_SHIFT ) & MSB_MASK )

typedef enum {
    EEPROM_OK,
    EEPROM_INIT_ERROR,
    EEPROM_NOT_VALIDE_ADRESS,
    EEPROM_READ_ERROR,
    EEPROM_WRITE_ERROR,
    EEPROM_ACCESS_ERROR,
    EEPROM_BUSY,
} EERPOM_ERROR_CODE_t;

typedef struct
{
   uint8_t direction;
   uint16_t start_addres;
   uint16_t size;
   uint8_t Data[SECTOR_SIZE  ];

} EEPROM_SECTOR_t;

typedef struct
{
   uint8_t ucTaskNatificationIndex;
   uint8_t BusyFlag;
   uint8_t direciorn;
   uint8_t ADDR[2];
   uint8_t DevAdrres;
   uint16_t data_address;
   uint8_t DataLength;
   uint8_t Index;
   uint8_t * ReciveBuffer;
   I2C_NAME_t dev;
   TaskHandle_t NotifyTaskHeandle;
   uint8_t DMA_TX;
   EERPOM_ERROR_CODE_t (*I2C_Master_Recive_func) (  u8 , u16,  u8 * , u16 , u32 ,u8 );
   EERPOM_ERROR_CODE_t (*I2C_Master_Transmit_func)( u8 , u16,  u8 * , u16 , u32 ,u8 );
} EEPOROM;



void vInitEEPROM();
EEPOROM * xGetEEPROM();
EERPOM_ERROR_CODE_t eEEPROMWr(  EEPROM_ADRESS_TYPE addr, uint8_t * data, EEPROM_ADRESS_TYPE len , uint8_t NotifyIndex );
EERPOM_ERROR_CODE_t eEEPROMRd(  EEPROM_ADRESS_TYPE addr, uint8_t * data,   EEPROM_ADRESS_TYPE len , uint8_t NotifyIndex );
uint8_t bReadEEPROM( EEPROM_ADRESS_TYPE addr, uint8_t NotifyIndex );

#endif /* HW_LIB_HW_LIB_EEPROM_H_ */
