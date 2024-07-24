/*
 * hal_i2c.c
 *
 *  Created on: 20 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_i2c.h"
#include "hw_lib_eeprom.h"
#include "hal_gpio.h"

#if MCU == APM32
#include "apm32f4xx_dma.h"


EEPOROM *   pEEPROM 			__SECTION(RAM_SECTION_CCMRAM);

static EERPOM_ERROR_CODE_t I2C_Master_ReviceIT(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );
static EERPOM_ERROR_CODE_t I2C_Master_TransmitIT(  u8 DevAdrees, u16 data_addres, u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );


static inline void EEPROM_I2C_ENABLE()          { pEEPROM->dev->CTRL1_B.I2CEN = SET;   }
static inline void EEPROM_I2C_START()          { pEEPROM->dev->CTRL1_B.START = SET;  }
static inline void EEPROM_I2C_STOP()           { pEEPROM->dev->CTRL1_B.STOP = SET; }
static inline void EEPROM_I2C_ACK_ENABLE()      { pEEPROM->dev->CTRL1_B.ACKEN = SET;}
static inline void EEPROM_I2C_ACK_DISABLE()     { pEEPROM->dev->CTRL1_B.ACKEN = RESET;}
static inline void EEPROM_I2C_SEND( u8 DATA)    { pEEPROM->dev->DATA_B.DATA = DATA; };
static inline void  EERPOM_I2C_SEND_ADDR_TRANS( u8 DATA)   { pEEPROM->dev->DATA_B.DATA = ( DATA &  0xFFFE);}
static inline void  EERPOM_I2C_SEND_ADDR_RECIEVE( u8 DATA)  { pEEPROM->dev->DATA_B.DATA= (DATA | 0x0001);}
static inline void  I2C_IT_ENABLE( u16 DATA )                {  pEEPROM->dev->CTRL2 |= (DATA); }
static inline void I2C_IT_DISABLE( u16 DATA )               {  pEEPROM->dev->CTRL2 &= ~ (DATA); }
static inline void HAL_I2C_AND_IT_Disable()
{
	pEEPROM->dev->CTRL2 &= ~( I2C_INT_EVT | I2C_INT_BUF ); //  Disable the specified I2C interrupts.
	pEEPROM->dev->CTRL1_B.I2CEN = RESET;     			   //  Disable I2C
}
#define EEPROM_I2C_READ_DATA()  pEEPROM->dev->DATA_B.DATA

void InitI2CDMA( I2C_NAME_t i2c)
{

    if ( i2c == I2C_1)
    {
    	RCM->APB1CLKEN |= RCM_APB1_PERIPH_I2C1;
    	RCM->AHB1RST   |= RCM_APB1_PERIPH_I2C1;
    	RCM->AHB1RST &= (uint32_t)~RCM_APB1_PERIPH_I2C1;
    }
    else
    {
    	RCM->APB1CLKEN |= RCM_APB1_PERIPH_I2C2;
    	RCM->AHB1RST   |= RCM_APB1_PERIPH_I2C2;
    	 RCM->AHB1RST &= (uint32_t)~RCM_APB1_PERIPH_I2C2;

    }

    uint16_t temp = 0, freq = 0;
    uint32_t PCLK1 = 8000000, PCLK2 = 0;
    uint16_t result = 0x04;

    /* I2C CTRL2 Configuration */
    RCM_ReadPCLKFreq(&PCLK1, &PCLK2);
    freq = PCLK1 / 1000000;
    i2c->CTRL2_B.CLKFCFG = freq;

    /* I2C CLKCTRL Configuration */
    i2c->CTRL1_B.I2CEN = BIT_RESET;
    /* Configure speed in fast mode */

    result = (PCLK1 / (400000 * 25));
    result |= I2C_DUTYCYCLE_16_9;
    if ((result & 0x0FFF) == 0)
    {
         result |= 0x0001;
    }

     temp |= (uint16_t)(result | 0x8000);
     i2c->RISETMAX = ((((freq) * 300) / 1000) + 1);

     i2c->CLKCTRL = temp;
     i2c->CTRL1_B.I2CEN = BIT_SET;
     /* i2c CTRL1 Configuration  */
     i2c->CTRL1_B.ACKEN = BIT_RESET;
     i2c->CTRL1_B.SMBTCFG = BIT_RESET;
     i2c->CTRL1_B.SMBEN = BIT_RESET;
     i2c->CTRL1 |= I2C_MODE_I2C;
     i2c->CTRL1_B.ACKEN = I2C_ACK_ENABLE;
     i2c->SADDR1 = I2C_ACK_ADDRESS_7BIT;


   pEEPROM =xGetEEPROM();
   pEEPROM->dev = i2c;
   pEEPROM->BusyFlag = 0;
   pEEPROM->I2C_Master_Recive_func =    I2C_Master_ReviceIT;
   pEEPROM->I2C_Master_Transmit_func =  I2C_Master_TransmitIT;

    if (i2c == I2C_1)
    {
    		NVIC_EnableIRQRequest(I2C1_EV_IRQn, I2C_PRIOR, I2C_SUBPRIOR);
    		NVIC_EnableIRQRequest(I2C1_ER_IRQn, I2C_PRIOR, I2C_SUBPRIOR);
    }
    else
    if (i2c == I2C_2)
    {
    		NVIC_EnableIRQRequest(I2C2_EV_IRQn, 5, 5);
    	    NVIC_EnableIRQRequest(I2C2_ER_IRQn, 5, 5);
    }
    i2c->SADDR2_B.ADDRNUM = RESET;
    EEPROM_I2C_ENABLE();

   // 	vDMAI2CEnable();



}


static EERPOM_ERROR_CODE_t I2C_Master_ReviceIT( u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	u8 i =0;
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
	pEEPROM->BusyFlag		 = 1;
	pEEPROM->ucTaskNatificationIndex = TNI;
	pEEPROM->Index          = 0;
    pEEPROM->DataLength     = data_size;
	pEEPROM->ReciveBuffer   = data;
	pEEPROM->DevAdrres      = DevAdrees;
	pEEPROM->data_address   = data_addres;
	pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
	pEEPROM->I2C_State = I2C_MASTER_RECIVE_START;
	EEPROM_I2C_ENABLE();
	for (;i<0xFF;i++)
	{
	   if (pEEPROM->dev->STS2_B.BUSBSYFLG == RESET) break;
	}
	if (i < 0xFF)
	{
	    EEPROM_I2C_ACK_ENABLE();
	    EEPROM_I2C_START();
	    I2C_IT_ENABLE( I2C_INT_EVT | I2C_INT_BUF );
	    uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout );
	    res = (exit_code == 0x01  )? (EEPROM_OK) : (EEPROM_WRITE_ERROR) ;
	    pEEPROM->BusyFlag = 0;
	    return (res);
	 }
	 else
	 {
	     return (EEPROM_READ_ERROR);
	 }
}

static EERPOM_ERROR_CODE_t I2C_Master_TransmitIT(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	u8 i =0;
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
	pEEPROM->BusyFlag = 1;
	pEEPROM->ucTaskNatificationIndex = TNI;
	pEEPROM->Index          = 0;
	pEEPROM->DataLength     = data_size;
    pEEPROM->ReciveBuffer   = data;
	pEEPROM->DevAdrres      = DevAdrees;
    pEEPROM->data_address   = data_addres;
    pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
    pEEPROM->I2C_State = I2C_MASTER_TRANSMIT_START;
    EEPROM_I2C_ENABLE();
    for (;i<0xFF;i++)
    {
    	if (pEEPROM->dev->STS2_B.BUSBSYFLG == RESET) break;
    }
    if (i < 0xFF)
    {
    	EEPROM_I2C_START();
    	I2C_IT_ENABLE(  I2C_INT_EVT | I2C_INT_BUF);
    	uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout);
    	res = (exit_code > 0  )? (EEPROM_OK) : (EEPROM_READ_ERROR);
    	return (res);
    }
    else
    {
    	return (EEPROM_READ_ERROR);
    }
}


static void I2C_FSM()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     uint16_t int_flags =   pEEPROM->dev->STS1;
     switch(pEEPROM->I2C_State)
     {
     case  I2C_MASTER_RECIVE_START:
    	 	 if (int_flags & 0x0001)
    	 	 {
    	 		     EERPOM_I2C_SEND_ADDR_TRANS( pEEPROM->DevAdrres |  GET_ADDR_MSB(pEEPROM->data_address));
    	 		 	 pEEPROM->I2C_State = I2C_MASTER_RECIVE_WRITE_ADDR;
    	 	 }
    	 	 break;
     	 case I2C_MASTER_RECIVE_WRITE_ADDR:
     		 if (int_flags & 0x0002)
     		 {
     			int_flags = pEEPROM->dev->STS2;
     			 EEPROM_I2C_SEND(pEEPROM->data_address & 0xFF) ;
     			 pEEPROM->I2C_State = I2C_MASTER_RECIVE_DA_WRITE;
     		 }
     		 break;
     	 case I2C_MASTER_RECIVE_DA_WRITE:
     		 if (int_flags & 0x0080)
     		 {
     			EEPROM_I2C_START();
     			 pEEPROM->I2C_State = I2C_MASTER_RECIVE_ADDR;
     		 }
     		 break;
         case I2C_MASTER_RECIVE_ADDR:
        	 if (int_flags & 0x0001)
        	 {
        		 EERPOM_I2C_SEND_ADDR_RECIEVE( pEEPROM->DevAdrres);
        		 pEEPROM->I2C_State = I2C_MASTER_RECIVE_MODE;
        	 }
        	 break;

         case I2C_MASTER_RECIVE_MODE:
        	 if (int_flags & 0x0002)
        	 {
        		 int_flags = pEEPROM->dev->STS2;
        		 pEEPROM->I2C_State = I2C_MASTER_RECIEVE;
        	 }
        	 break;
     	 case I2C_MASTER_RECIEVE:
     		 if (int_flags & 0x0040)
     		 {
     			if (pEEPROM->Index < (pEEPROM->DataLength-1))
     			{
     			    pEEPROM->ReciveBuffer[pEEPROM->Index++] = EEPROM_I2C_READ_DATA() ;
     			}
     			else
     			{
     				EEPROM_I2C_ACK_DISABLE();
     				pEEPROM->ReciveBuffer[pEEPROM->Index++] = EEPROM_I2C_READ_DATA();
     				EEPROM_I2C_STOP();
     			    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex, 0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
     				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
     				pEEPROM->I2C_State = I2C_IDLE;
     				HAL_I2C_AND_IT_Disable();
     			}
     		 }
     		 break;
     	 case  I2C_MASTER_TRANSMIT_START:
     		if (int_flags & 0x0001)
     		{
     			 EERPOM_I2C_SEND_ADDR_TRANS( pEEPROM->DevAdrres |  GET_ADDR_MSB(pEEPROM->data_address));
     			 pEEPROM->I2C_State = I2C_MASTER_TRANSMIT_ADDR;
     		}
     		 break;
     	 case I2C_MASTER_TRANSMIT_ADDR:
     		if  ( int_flags & 0x0400)
     		 {
     			 pEEPROM->dev->STS1 = (uint16_t)~(I2C_INT_FLAG_AE & 0x00FFFFFF);
     		     EEPROM_I2C_START();
     		     pEEPROM->I2C_State = I2C_MASTER_TRANSMIT_START;
     		  }
     		 else
     		if (int_flags & 0x0002)
     		{
     			 int_flags = pEEPROM->dev->STS2;
     			 EEPROM_I2C_SEND (pEEPROM->data_address & 0xFF) ;
     			 pEEPROM->I2C_State = I2C_MASTER_TRANSMIT_NEXT;
     		}
     		 break;
     	 case I2C_MASTER_TRANSMIT_NEXT:
     		 if (int_flags & 0x0080)
     		 {
     			if (pEEPROM->Index < pEEPROM->DataLength )
     			{
     				EEPROM_I2C_SEND( pEEPROM->ReciveBuffer[pEEPROM->Index] );
     			    pEEPROM->Index++;
     			}
     			else
     				 pEEPROM->I2C_State = I2C_MASTER_TRANSMIT_LAST;
     		 }
     		 break;
     	 case  I2C_MASTER_TRANSMIT_LAST:
     		 if (int_flags & 0x0004)
     		 {
     			 EEPROM_I2C_STOP();
     			 xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex, 0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
     			 portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
     			 pEEPROM->I2C_State = I2C_IDLE;
     			 HAL_I2C_AND_IT_Disable();
     		 }
     		 break;
     	 default:
     		 break;

     }

}



void I2C1_EV_IRQHandler( void )
{
	I2C_FSM();
}
void I2C1_ER_IRQHandler ( void )
{
	I2C_FSM();
}
void  I2C2_EV_IRQHandler( void )
{
	I2C_FSM();
}
void I2C2_ER_IRQHandler ( void )
{
	I2C_FSM();
}

#endif
