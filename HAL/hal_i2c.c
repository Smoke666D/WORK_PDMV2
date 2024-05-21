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
#endif

EEPOROM *   pEEPROM 			__SECTION(RAM_SECTION_CCMRAM);
static void vDMAI2CEnable( void );
static EERPOM_ERROR_CODE_t I2C_Master_ReviceDMA(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );
static EERPOM_ERROR_CODE_t I2C_Master_TransmitDMA(  u8 DevAdrees,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );
static EERPOM_ERROR_CODE_t I2C_Master_ReviceIT(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );
static EERPOM_ERROR_CODE_t I2C_Master_TransmitIT(  u8 DevAdrees,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );


void InitI2CDMA( I2C_NAME_t i2c)
{

#if	MCU == CH32V2
	NVIC_InitTypeDef      NVIC_InitStructure = {0};
	if ( i2c == I2C_1)
	{
		HAL_InitGpioAF(  I2C1_Port , I2C1_SCL_Pin  | I2C1_SDA_Pin , 0, MODE_OUT_PP );
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
	}
	else
	if (i2c == I2C_2)
	{
	    HAL_InitGpioAF(  I2C1_Port , I2C1_SCL_Pin  | I2C1_SDA_Pin , 0, MODE_OUT_PP );
	    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );

	}
    I2C_InitTypeDef I2C_InitTSturcture={0};
    I2C_InitTSturcture.I2C_ClockSpeed = 100000;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(i2c, &I2C_InitTSturcture );
    I2C_Cmd( i2c, ENABLE );
#endif
#if MCU == APM32
    if ( i2c == I2C_1)
    		RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);
    else
    if (i2c == I2C_2)
    {
    		HAL_InitGpioAF( PORT_F, GPIO_PIN_0, GPIO_AF_I2C2, GPIO_OTYPE_OD );
    		HAL_InitGpioAF( PORT_F, GPIO_PIN_1, GPIO_AF_I2C2 ,GPIO_OTYPE_OD );
    		RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C2);

    }
    I2C_Config_T I2C_InitTSturcture;
    I2C_InitTSturcture.clockSpeed 	= 100000;
    I2C_InitTSturcture.mode 		= I2C_MODE_I2C;
    I2C_InitTSturcture.dutyCycle 	= I2C_DUTYCYCLE_16_9;
    I2C_InitTSturcture.ack 			= I2C_ACK_ENABLE;
    I2C_InitTSturcture.ackAddress 	= I2C_ACK_ADDRESS_7BIT;
    I2C_Config(i2c,&I2C_InitTSturcture );
#endif
   pEEPROM =xGetEEPROM();
   pEEPROM->dev = i2c;
   pEEPROM->BusyFlag = 0;
#if MCU == APM32
   pEEPROM->I2C_Master_Recive_func = I2C_Master_ReviceDMA;
   pEEPROM->I2C_Master_Transmit_func =  I2C_Master_TransmitDMA;
#endif


#if MCU == CH32V2

   pEEPROM->I2C_Master_Recive_func = I2C_Master_ReviceIT;
   pEEPROM->I2C_Master_Transmit_func =  I2C_Master_TransmitIT;
   if (i2c == I2C_1)
       {
	   	   NVIC_InitStructure.NVIC_IRQChannel =  I2C1_EV_IRQn ;
	       NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	       NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	       NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	       NVIC_Init(&NVIC_InitStructure);
	       NVIC_InitStructure.NVIC_IRQChannel =  I2C1_ER_IRQn;
	       NVIC_Init(&NVIC_InitStructure);

       }
       else
       if (i2c == I2C_2)
       {
    	   NVIC_InitStructure.NVIC_IRQChannel =  I2C2_EV_IRQn ;
    	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    	   NVIC_Init(&NVIC_InitStructure);
    	   NVIC_InitStructure.NVIC_IRQChannel =  I2C2_ER_IRQn;
    	   NVIC_Init(&NVIC_InitStructure);
       }
       I2C_DualAddressCmd(pEEPROM->dev,DISABLE);
       I2C_Cmd(pEEPROM->dev,ENABLE);
#endif
#if MCU == APM32
    if (i2c == I2C_1)
    {
    		NVIC_EnableIRQRequest(I2C1_EV_IRQn, 5, 0);
    		NVIC_EnableIRQRequest(I2C1_ER_IRQn, 5, 0);
    }
    else
    if (i2c == I2C_2)
    {
    		NVIC_EnableIRQRequest(I2C2_EV_IRQn, 5, 0);
    	    NVIC_EnableIRQRequest(I2C2_ER_IRQn, 5, 0);
    }
    I2C_DisableDualAddress(pEEPROM->dev);
    I2C_Enable(pEEPROM->dev);
    vDMAI2CEnable();
#endif

}



static EERPOM_ERROR_CODE_t I2C_Master_ReviceDMA(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{

	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
#if MCU == APM32
	if ( pEEPROM->BusyFlag == 0 )
	{
		pEEPROM->BusyFlag = 1;
		pEEPROM->ucTaskNatificationIndex = TNI;
		pEEPROM->Index          = data_addres;
		pEEPROM->DataLength     = data_size;
		pEEPROM->ReciveBuffer   = data;
		pEEPROM->DevAdrres      = DevAdrees;
		pEEPROM->direciorn =     DIR_TRANSMIT;
		pEEPROM->DMA_TX = 0;
        pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
	    I2C_EnableInterrupt( pEEPROM->dev ,  I2C_INT_EVT | I2C_INT_BUF);
	    I2C_EnableAcknowledge(  pEEPROM->dev);
	    I2C_EnableDMALastTransfer( pEEPROM->dev );
	    DMA_ConfigDataNumber(DMA1_Stream3, data_size);
	    DMA_ConfigMemoryTarget(DMA1_Stream3, ( uint32_t ) data, DMA_MEMORY_0);
	    DMA_Enable(DMA1_Stream3);
		I2C_EnableGenerateStart(  pEEPROM->dev);
		uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout);
	    res = (exit_code > 0  )? (EEPROM_OK) : (EEPROM_READ_ERROR);
	    pEEPROM->BusyFlag = 0;
	  }
#endif
	  return (res);
}


static EERPOM_ERROR_CODE_t I2C_Master_TransmitDMA(  u8 DevAdrees,   u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
#if MCU == APM32
	if ( pEEPROM->BusyFlag == 0 )
	{
		pEEPROM->BusyFlag = 1;
		pEEPROM->ucTaskNatificationIndex = TNI;
		pEEPROM->Index          = 0;
		pEEPROM->DataLength     = data_size;
		pEEPROM->ReciveBuffer   = data;
		pEEPROM->DevAdrres      = DevAdrees;
		pEEPROM->direciorn      = DIR_TRANSMIT;
		pEEPROM->DMA_TX = 1;
        pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
        I2C_DisableAcknowledge(pEEPROM->dev);
        I2C_EnableInterrupt( pEEPROM->dev  , I2C_INT_EVT | I2C_INT_ERR );
        I2C_DisableInterrupt( pEEPROM->dev ,  I2C_INT_BUF );
        DMA_ConfigDataNumber(DMA1_Stream7, data_size);
        DMA_ConfigMemoryTarget(DMA1_Stream7, (uint32_t) data, DMA_MEMORY_0);
        DMA_ClearIntFlag(DMA1_Stream7, DMA_INT_TEIFLG7 |  DMA_INT_FEIFLG7 );
        DMA_Enable(DMA1_Stream7);
        I2C_EnableGenerateStart( pEEPROM->dev );
        uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout );
        res = (exit_code == 0x01  )? (EEPROM_OK) : (EEPROM_WRITE_ERROR) ;
        pEEPROM->BusyFlag = 0;
	  }
#endif
	  return (res);
}


static EERPOM_ERROR_CODE_t I2C_Master_ReviceIT(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{

	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
#if MCU == CH32V2
	if ( pEEPROM->BusyFlag == 0 )
	{
		pEEPROM->BusyFlag = 1;
		pEEPROM->ucTaskNatificationIndex = TNI;
		pEEPROM->Index          = data_addres;
		pEEPROM->DataLength     = data_size;
		pEEPROM->ReciveBuffer   = data;
		pEEPROM->DevAdrres      = DevAdrees;
		pEEPROM->direciorn =     DIR_TRANSMIT;
		pEEPROM->DMA_TX = 0;
        pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
        I2C_ITConfig( pEEPROM->dev  , I2C_IT_EVT | I2C_IT_ERR | I2C_IT_BUF , ENABLE);
	    I2C_AcknowledgeConfig(  pEEPROM->dev, ENABLE);
	    I2C_GenerateSTART(  pEEPROM->dev, ENABLE);
		uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout);
	    res = (exit_code > 0  )? (EEPROM_OK) : (EEPROM_READ_ERROR);
	    pEEPROM->BusyFlag = 0;
	  }
#endif
	  return (res);
}


static EERPOM_ERROR_CODE_t I2C_Master_TransmitIT(  u8 DevAdrees,   u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
#if MCU == CH32V2
	if ( pEEPROM->BusyFlag == 0 )
	{
		pEEPROM->BusyFlag = 1;
		pEEPROM->ucTaskNatificationIndex = TNI;
		pEEPROM->Index          = 0;
		pEEPROM->DataLength     = data_size;
		pEEPROM->ReciveBuffer   = data;
		pEEPROM->DevAdrres      = DevAdrees;
		pEEPROM->direciorn      = DIR_TRANSMIT;
		pEEPROM->DMA_TX = 1;
        pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
        I2C_AcknowledgeConfig(  pEEPROM->dev, DISABLE);
        I2C_ITConfig( pEEPROM->dev  , I2C_IT_EVT | I2C_IT_ERR | I2C_IT_BUF , ENABLE);
        I2C_GenerateSTART(  pEEPROM->dev, ENABLE);
        uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout );
        res = (exit_code == 0x01  )? (EEPROM_OK) : (EEPROM_WRITE_ERROR) ;
        pEEPROM->BusyFlag = 0;
	  }
#endif
	  return (res);
}


static void vDMAI2CEnable(void )
{
/* Enable DMA clock */
#if MCU == APM32
   DMA_Config_T dmaConfig;
   RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_DMA1);
   NVIC_EnableIRQRequest(DMA1_STR3_IRQn, 6, 0);
   DMA_Disable(DMA1_Stream3);
   dmaConfig.bufferSize = 0;
   dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
   dmaConfig.peripheralDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
   dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
   dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
   dmaConfig.loopMode = DMA_MODE_NORMAL;
   dmaConfig.priority =  DMA_PRIORITY_HIGH;
   /* read from peripheral*/
   dmaConfig.dir = DMA_DIR_PERIPHERALTOMEMORY;
   dmaConfig.peripheralBaseAddr = (uint32_t)&I2C2->DATA;
   dmaConfig.memoryBaseAddr =   0;
   dmaConfig.channel           = DMA_CHANNEL_7;
   dmaConfig.fifoMode          = DMA_FIFOMODE_DISABLE;
   dmaConfig.fifoThreshold     = DMA_FIFOTHRESHOLD_FULL;
   dmaConfig.peripheralBurst   = DMA_PERIPHERALBURST_SINGLE;
   dmaConfig.memoryBurst       = DMA_MEMORYBURST_SINGLE;
   DMA_Config(DMA1_Stream3, &dmaConfig);
   DMA_ClearIntFlag(DMA1_Stream3, DMA_INT_TCIFLG3 | DMA_INT_TEIFLG3 );
   DMA_ClearStatusFlag(DMA1_Stream3, DMA_FLAG_TEIFLG3 | DMA_FLAG_DMEIFLG3 );
   DMA_EnableInterrupt(DMA1_Stream3, DMA_INT_TCIFLG );
   NVIC_EnableIRQRequest(DMA1_STR7_IRQn, 6, 0);
   DMA_Disable(DMA1_Stream7);
   dmaConfig.dir = DMA_DIR_MEMORYTOPERIPHERAL;
   DMA_Config(DMA1_Stream7, &dmaConfig);
   DMA_ClearIntFlag(DMA1_Stream7, DMA_INT_TCIFLG7 | DMA_INT_TEIFLG7 );
   DMA_ClearStatusFlag(DMA1_Stream7, DMA_FLAG_TEIFLG7 | DMA_FLAG_DMEIFLG7 );
   DMA_EnableInterrupt(DMA1_Stream7, DMA_INT_TCIFLG );

#endif
 }

#if MCU == APM32
void DMA1_STR3_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA1_Stream3, DMA_INT_TCIFLG3) ==SET)
	{
		DMA_ClearIntFlag(DMA1_Stream3, DMA_INT_TCIFLG3);
		I2C_DisableDMA( pEEPROM->dev );
	 	I2C_EnableGenerateStop(  pEEPROM->dev );
	 	DMA_Disable(DMA1_Stream3);
	    portBASE_TYPE xHigherPriorityTaskWoken =  pdFALSE;
	    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

#endif


#if MCU == APM32
void DMA1_STR7_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA1_Stream7, DMA_INT_TCIFLG7) ==SET)
	{
		DMA_ClearIntFlag(DMA1_Stream7, DMA_INT_TCIFLG7);
		I2C_DisableDMA( pEEPROM->dev );
	 	DMA_Disable(DMA1_Stream7);
	}
}
#endif

static void I2C_FSM()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if MCU == CH32V2
    uint16_t int_flags =   I2C_ReadRegister( pEEPROM->dev,  I2C_Register_STAR1 );


    //EV5 STATE
     if ( (int_flags  & (I2C_FLAG_SB & 0xFF) ) )
     {
         if (pEEPROM->direciorn  == DIR_TRANSMIT )
        	   I2C_Send7bitAddress(  pEEPROM->dev, pEEPROM->DevAdrres |  GET_ADDR_MSB(pEEPROM->Index), pEEPROM->direciorn );
         else
        	   I2C_Send7bitAddress(  pEEPROM->dev, pEEPROM->DevAdrres, pEEPROM->direciorn );
    	 return;
     }
     //EV6
        if ( ( int_flags  & (I2C_FLAG_ADDR   & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_TRANSMIT  ) )
        {

        	if (pEEPROM->DMA_TX == 0)
        	{
        		 I2C_ReadRegister( pEEPROM->dev,  I2C_Register_STAR2 );
        		 I2C_SendData(pEEPROM->dev, (u8)(pEEPROM->Index & 0x00FF));
        	}
        	else
        	{
        		I2C_ReadRegister( pEEPROM->dev,  I2C_Register_STAR2 );
        		I2C_SendData( pEEPROM->dev, pEEPROM->ReciveBuffer[pEEPROM->Index] );
        		pEEPROM->Index++;
        	}
    	    return;
        }
        if ( ( int_flags  & (I2C_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_RECIEVE ) )
        {
        	  I2C_ReadRegister(pEEPROM->dev,  I2C_Register_STAR2);
        	  return;
        }
        if (( int_flags  & ((I2C_FLAG_TXE )   & 0xFF) )  && (pEEPROM->DMA_TX == 1))
        {
        	if (pEEPROM->Index < pEEPROM->DataLength )
        	 {
        	     I2C_SendData( pEEPROM->dev, pEEPROM->ReciveBuffer[pEEPROM->Index] );
        	     pEEPROM->Index++;
        	     return;
        	 }

        }

        if (( int_flags  & ((I2C_FLAG_TXE  | I2C_FLAG_BTF )   & 0xFF) ) )
        {
          	if (pEEPROM->DMA_TX == 0)
          	{
          		pEEPROM->direciorn =   DIR_RECIEVE;
          		I2C_GenerateSTART(  pEEPROM->dev, ENABLE);
          	}
          	else
          	{
          		I2C_GenerateSTOP( pEEPROM->dev, ENABLE );
          		xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
          		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

          	}
          	return;
        }
        if (( int_flags  & ((I2C_FLAG_RXNE | I2C_FLAG_BTF   )   & 0xFF) ) )
        {
        	 if ( pEEPROM->Index < (pEEPROM->DataLength-1))
        	 {
        	     I2C_AcknowledgeConfig(pEEPROM->dev, ENABLE );
        	     pEEPROM->ReciveBuffer[pEEPROM->Index++] = I2C_ReceiveData( pEEPROM->dev);
        	 }
        	 else
        	 {
        		  I2C_AcknowledgeConfig(pEEPROM->dev, DISABLE );
        		  pEEPROM->ReciveBuffer[pEEPROM->Index++] = I2C_ReceiveData( pEEPROM->dev);
        	      I2C_GenerateSTOP( pEEPROM->dev, ENABLE );
        	      xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
        	      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        	 }
             return;
        }

#endif

#if MCU == APM32
    uint16_t int_flags =   I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS1 );
   //EV5 STATE
    if ( (int_flags  & (I2C_INT_FLAG_START & 0xFF) ) )
    {
       I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS2 );
       if (pEEPROM->direciorn  == DIR_TRANSMIT )
       {
    	   I2C_Tx7BitAddress(  pEEPROM->dev, pEEPROM->DevAdrres |  GET_ADDR_MSB(pEEPROM->Index), pEEPROM->direciorn );
       }
       else
       {
    	   I2C_Tx7BitAddress(  pEEPROM->dev, pEEPROM->DevAdrres, pEEPROM->direciorn );
       }
	   return;
    }
    //EV6
    if ( ( int_flags  & (I2C_INT_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_RECIEVE ) )
    {
    	 I2C_DisableInterrupt(pEEPROM->dev ,  I2C_INT_BUF );
    	 I2C_EnableDMA( pEEPROM->dev );
    	 DMA_ClearIntFlag(DMA1_Stream3, DMA_INT_TEIFLG3 );
	     I2C_ReadRegister(pEEPROM->dev,  I2C_REGISTER_STS2 );
	     return;
    }
    if ( ( int_flags  & (I2C_INT_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_TRANSMIT )  )
    {

    	if (pEEPROM->DMA_TX == 0)
    	{
    		I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS2 );
    		I2C_TxData( pEEPROM->dev , (pEEPROM->Index & 0xFF) );
    	}
    	else
    	{
    		 I2C_EnableDMA( pEEPROM->dev );
    		 I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS2 );
    	}
    	 return;
    }

    if (( int_flags  & ((I2C_INT_FLAG_TXBE  | I2C_INT_FLAG_BTC)   & 0xFF) ) )
    {
    	if (pEEPROM->DMA_TX == 0)
    	{
    		pEEPROM->direciorn =   DIR_RECIEVE;
    		I2C_EnableGenerateStart(  pEEPROM->dev  );
    	}

    	return;

    }


#endif
}

void I2C_ERROR_FSM()
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
#if MCU == APM32
	 I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS2 );
	 uint16_t int_flags =  I2C_ReadRegister( pEEPROM->dev,  I2C_REGISTER_STS1 );
	 if ( ( int_flags & I2C_INT_FLAG_AE) == ( I2C_INT_FLAG_AE & 0xFFFF ) )
	 {
	    I2C_ClearIntFlag( pEEPROM->dev,I2C_INT_FLAG_AE);
	    I2C_EnableGenerateStop( pEEPROM->dev );

	    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x02, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	    return;
	 }
#endif
#if MCU == CH32V2
	 I2C_ReadRegister(pEEPROM->dev,  I2C_Register_STAR2);
		 uint16_t int_flags =  I2C_ReadRegister(pEEPROM->dev,  I2C_Register_STAR1);
		 if ( ( int_flags & I2C_FLAG_AF) & 0xFFFF )
		 {
			 I2C_ClearITPendingBit( pEEPROM->dev,I2C_IT_AF);
			 I2C_GenerateSTOP( pEEPROM->dev, ENABLE );
		    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x02, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
		    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		    return;
		 }



#endif

}



void I2C1_EV_IRQHandler( void )
{
	I2C_FSM();
}
void I2C1_ER_IRQHandler ( void )
{
	I2C_ERROR_FSM();
}
void  I2C2_EV_IRQHandler( void )
{
	I2C_FSM();
}
void I2C2_ER_IRQHandler ( void )
{
	I2C_ERROR_FSM();
}

