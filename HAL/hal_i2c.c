/*
 * hal_i2c.c
 *
 *  Created on: 20 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_i2c.h"
#include "hw_lib_eeprom.h"
#include "apm32f4xx_dma.h"

#if PLATFORM == APM32
static I2C_t I2C_NAME[] = { I2C1, I2C2};
#endif

EEPOROM *   pEEPROM 			__SECTION(RAM_SECTION_CCMRAM);

void vDMAI2CEnable( );
static EERPOM_ERROR_CODE_t I2C_Master_Recive(  u8 DevAdrees, u8 * data, u16 data_size, u32 timeout, uint8_t TNI);
static EERPOM_ERROR_CODE_t I2C_Master_Transmit(  u8 DevAdrees,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI);
static EERPOM_ERROR_CODE_t I2C_Master_ReviceDMA(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );
static EERPOM_ERROR_CODE_t I2C_Master_TransmitDMA(  u8 DevAdrees,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  );

void InitI2CDMA( I2C_NAME_t i2c)
{

#if	PLATFORM == RISC
    I2C_InitTypeDef I2C_InitTSturcture={0};
    I2C_InitTSturcture.I2C_ClockSpeed = 100000;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2Cx, &I2C_InitTSturcture );
    I2C_Cmd( I2Cx, ENABLE );
#endif
#if PLATFORM == APM32
    switch (i2c)
    {
    	case I2C_1:
    		RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);
    		break;
    	case I2C_2:
    		HAL_InitGpioAF( PORT_F, GPIO_PIN_0, GPIO_AF_I2C2, GPIO_OTYPE_OD );
    		HAL_InitGpioAF( PORT_F, GPIO_PIN_1, GPIO_AF_I2C2 ,GPIO_OTYPE_OD );
    		RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C2);
    		break;
    }
    I2C_Config_T I2C_InitTSturcture;
    I2C_InitTSturcture.clockSpeed 	= 100000;
    I2C_InitTSturcture.mode 		= I2C_MODE_I2C;
    I2C_InitTSturcture.dutyCycle 	= I2C_DUTYCYCLE_16_9;
    I2C_InitTSturcture.ack 			= I2C_ACK_ENABLE;
    I2C_InitTSturcture.ackAddress 	= I2C_ACK_ADDRESS_7BIT;
    I2C_Config(I2C_NAME[i2c],&I2C_InitTSturcture );
#endif
   pEEPROM =xGetEEPROM();
   pEEPROM->dev = i2c;
   pEEPROM->BusyFlag = 0;
   pEEPROM->I2C_Master_Recive_func = I2C_Master_ReviceDMA;
   pEEPROM->I2C_Master_Transmit_func =  I2C_Master_TransmitDMA;
   pEEPROM->DMA_RX = 0;
#if PLATFORM == RISC
    I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR , ENABLE);
#endif
#if PLATFORM == APM32
    switch (i2c)
    {
    	case I2C_1:
    		NVIC_EnableIRQRequest(I2C1_EV_IRQn, 5, 0);
    		NVIC_EnableIRQRequest(I2C1_ER_IRQn, 5, 0);
    		break;
    	case I2C_2:
    		NVIC_EnableIRQRequest(I2C2_EV_IRQn, 5, 0);
    	    NVIC_EnableIRQRequest(I2C2_ER_IRQn, 5, 0);

    }
    I2C_DisableDualAddress(I2C_NAME[i2c]);
    I2C_Enable(I2C_NAME[i2c]);
    vDMAI2CEnable( );
#endif
}



static EERPOM_ERROR_CODE_t I2C_Master_ReviceDMA(  u8 DevAdrees, u16 data_addres,  u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
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
	    I2C_EnableInterrupt(I2C_NAME[ pEEPROM->dev ],  I2C_INT_EVT | I2C_INT_BUF);
	    I2C_EnableAcknowledge( I2C_NAME[ pEEPROM->dev]);
	    I2C_EnableDMALastTransfer(I2C_NAME[ pEEPROM->dev ]);
	    DMA_ConfigDataNumber(DMA1_Stream3, data_size);
	    DMA_ConfigMemoryTarget(DMA1_Stream3, data,DMA_MEMORY_0);
	    DMA_Enable(DMA1_Stream3);
		I2C_EnableGenerateStart( I2C_NAME[ pEEPROM->dev] );
		uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout);
	    res = (exit_code > 0  )? (EEPROM_OK) : (EEPROM_READ_ERROR);
	   // pEEPROM->DMA_RX = 0;
	    pEEPROM->BusyFlag = 0;
	  }
	  return (res);
}


static EERPOM_ERROR_CODE_t I2C_Master_TransmitDMA(  u8 DevAdrees,   u8 * data, u16 data_size, u32 timeout,uint8_t TNI  )
{
	EERPOM_ERROR_CODE_t res = EEPROM_ACCESS_ERROR;
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
        I2C_DisableAcknowledge( I2C_NAME[ pEEPROM->dev]);
        I2C_EnableInterrupt(I2C_NAME[ pEEPROM->dev ] , I2C_INT_EVT );
        I2C_DisableInterrupt(I2C_NAME[ pEEPROM->dev ],  I2C_INT_BUF );
        DMA_ConfigDataNumber(DMA1_Stream7, data_size);
        DMA_ConfigMemoryTarget(DMA1_Stream7, data, DMA_MEMORY_0);
        DMA_ClearIntFlag(DMA1_Stream7, DMA_INT_TEIFLG7 |  DMA_INT_FEIFLG7 );
        DMA_Enable(DMA1_Stream7);
        I2C_EnableGenerateStart( I2C_NAME[ pEEPROM->dev] );
        uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout );
        res = (exit_code == 0x01  )? (EEPROM_OK) : (EEPROM_WRITE_ERROR) ;
        pEEPROM->BusyFlag = 0;
	  }
	  return (res);
}





void vDMAI2CEnable( )
{
/* Enable DMA clock */
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


 }

void DMA1_STR3_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA1_Stream3, DMA_INT_TCIFLG3) ==SET)
	{
		DMA_ClearIntFlag(DMA1_Stream3, DMA_INT_TCIFLG3);
		I2C_DisableDMA(I2C_NAME[ pEEPROM->dev ]);
	 	I2C_EnableGenerateStop(  I2C_NAME[ pEEPROM->dev] );
	 	DMA_Disable(DMA1_Stream3);
	    portBASE_TYPE xHigherPriorityTaskWoken =  pdFALSE;
	    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}


void DMA1_STR7_IRQHandler( void )
{
	if ( DMA_ReadIntFlag(DMA1_Stream7, DMA_INT_TCIFLG7) ==SET)
	{
		DMA_ClearIntFlag(DMA1_Stream7, DMA_INT_TCIFLG7);
		I2C_DisableDMA(I2C_NAME[ pEEPROM->dev ]);
	 	DMA_Disable(DMA1_Stream7);
	}
}






static EERPOM_ERROR_CODE_t I2C_Master_Transmit(  u8 DevAdrees,  u8 * data, u16 data_size, u32 timeout,  uint8_t TNI)
{
	EERPOM_ERROR_CODE_t res = EEPROM_WRITE_ERROR;
    if ( pEEPROM->BusyFlag == 0 )
	{
	    pEEPROM->BusyFlag = 1;
	    pEEPROM->ucTaskNatificationIndex = TNI;
    	pEEPROM->Index          = 0;
    	pEEPROM->DataLength     = data_size;
    	pEEPROM->ReciveBuffer   = data;
    	pEEPROM->DevAdrres      = DevAdrees;
    	pEEPROM->direciorn =     DIR_TRANSMIT;
    	pEEPROM->NotifyTaskHeandle = xTaskGetCurrentTaskHandle();
    	I2C_EnableAcknowledge( I2C_NAME[ pEEPROM->dev]);
    	I2C_DisableInterrupt(I2C_NAME[ pEEPROM->dev ], I2C_INT_ERR | I2C_INT_EVT | I2C_INT_BUF);
    	while (I2C_ReadStatusFlag(I2C_NAME[ pEEPROM->dev ], I2C_FLAG_BUSBSY));
    	I2C_EnableGenerateStart( I2C_NAME[ pEEPROM->dev] );
    	while (!I2C_ReadEventStatus(I2C_NAME[ pEEPROM->dev], I2C_EVENT_MASTER_MODE_SELECT));
    	I2C_Tx7BitAddress(  I2C_NAME[ pEEPROM->dev], pEEPROM->DevAdrres, pEEPROM->direciorn );
    	while (!I2C_ReadEventStatus(I2C_NAME[ pEEPROM->dev], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    	while (pEEPROM->Index < pEEPROM->DataLength )
    	{
    		I2C_TxData( I2C_NAME[ pEEPROM->dev] , pEEPROM->ReciveBuffer[pEEPROM->Index] );
    		pEEPROM->Index++;
    		if ((pEEPROM->Index ) >= pEEPROM->DataLength)
    		{
    			while (!I2C_ReadEventStatus(I2C_NAME[ pEEPROM->dev], I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    			I2C_EnableGenerateStop(I2C_NAME[ pEEPROM->dev]);
    		}
    		else
    		{
    			while (!I2C_ReadEventStatus(I2C_NAME[ pEEPROM->dev], I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    		}

    	}

    	 uint32_t exit_code = 0x01;

      /*  while (I2C_ReadStatusFlag(I2C_NAME[ pEEPROM->dev ], I2C_FLAG_BUSBSY) == SET);
    	I2C_ConfigPECPosition(I2C_NAME[ pEEPROM->dev], I2C_PEC_POSITION_CURRENT );
        I2C_EnableGenerateStart( I2C_NAME[ pEEPROM->dev] );
        uint32_t exit_code = ulTaskNotifyTakeIndexed( pEEPROM->ucTaskNatificationIndex, 0xFFFFFFFF, timeout);*/
        res = (exit_code == 0x01  )? (EEPROM_OK) : (EEPROM_WRITE_ERROR) ;
        pEEPROM->BusyFlag = 0;
    }
    return (res);
}


static void I2C_FSM()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if PLATFORM == RISC

    if ( I2C_GetITStatus(Dev.dev, I2C_IT_BTF) != RESET )
        {
            if ((Dev.Index >= Dev.DataLength) &&  (Dev.direciorn  == I2C_Direction_Transmitter))
            {
                I2C_GenerateSTOP( Dev.dev, ENABLE );
                xEventGroupSetBitsFromISR(  xEEPROMEventHandle, I2C_READY | I2C_STOP ,&xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

            }
            I2C_ClearITPendingBit( Dev.dev, I2C_IT_BTF);
        }
        if ( I2C_GetITStatus( Dev.dev, I2C_IT_SB ) != RESET )
        {
            I2C_Send7bitAddress( Dev.dev, Dev.DevAdrres, Dev.direciorn );
            I2C_ClearITPendingBit( Dev.dev, I2C_IT_SB );
            return;
        }
        if ( I2C_GetITStatus( Dev.dev, I2C_IT_ADDR) != RESET )
        {
             uint16_t temp = Dev.dev->STAR2;
             I2C_ClearITPendingBit( Dev.dev, I2C_IT_ADDR );
             Dev.Index = 0;
             return;
        }
        if ( I2C_GetITStatus(Dev.dev, I2C_IT_RXNE) != RESET )
        {
            if ( Dev.Index < Dev.DataLength)
            {
                FunctionalState state = (Dev.Index < (Dev.DataLength -1 )) ? ENABLE : DISABLE;
                I2C_AcknowledgeConfig(Dev.dev, state );
                Dev.ReciveBuffer[Dev.Index++] = I2C_ReceiveData( I2C2 );
            }
            else
            {
                I2C_GenerateSTOP( Dev.dev, ENABLE );
                xEventGroupSetBitsFromISR(  xEEPROMEventHandle, I2C_READY | I2C_STOP ,&xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
            }
            I2C_ClearITPendingBit( Dev.dev, I2C_IT_RXNE );
            return;
        }
        if ( I2C_GetITStatus(Dev.dev, I2C_IT_TXE) != RESET )
        {
            if (Dev.Index < Dev.DataLength)
            {
                I2C_SendData( Dev.dev, Dev.ReciveBuffer[Dev.Index] );
                Dev.Index++;
            }
            I2C_ClearITPendingBit( Dev.dev, I2C_IT_TXE);
            return;
        }
        if ( I2C_GetITStatus( Dev.dev, I2C_IT_AF) != RESET )
        {
             xEventGroupSetBitsFromISR(  xEEPROMEventHandle, I2C_READY | I2C_ACK_FAIL ,&xHigherPriorityTaskWoken);
             portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
             I2C_ClearITPendingBit( Dev.dev, I2C_IT_AF);
             return;
        }

#endif

#if PLATFORM == APM32
    uint16_t int_flags =   I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS1 );
   //EV5 STATE
    if ( (int_flags  & (I2C_INT_FLAG_START & 0xFF) ) )
    {
       I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS2 );
       if (pEEPROM->direciorn  == DIR_TRANSMIT )
    	   I2C_Tx7BitAddress(  I2C_NAME[ pEEPROM->dev], pEEPROM->DevAdrres |  GET_ADDR_MSB(pEEPROM->Index), pEEPROM->direciorn );
       else
    	   I2C_Tx7BitAddress(  I2C_NAME[ pEEPROM->dev], pEEPROM->DevAdrres, pEEPROM->direciorn );
	   return;
    }
    //EV6
    if ( ( int_flags  & (I2C_INT_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_RECIEVE ) )
    {
    	 I2C_DisableInterrupt(I2C_NAME[ pEEPROM->dev ],  I2C_INT_BUF );
    	 I2C_EnableDMA(I2C_NAME[ pEEPROM->dev ]);
    	 DMA_ClearIntFlag(DMA1_Stream3, DMA_INT_TEIFLG3 );
	     I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS2 );
	     return;
    }
    if ( ( int_flags  & (I2C_INT_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_TRANSMIT )  && (pEEPROM->DMA_TX == 0) )
    {
    	 I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS2 );
    	 I2C_TxData( I2C_NAME[ pEEPROM->dev] , (pEEPROM->Index & 0xFF) );
    	 return;
    }

    if ( ( int_flags  & (I2C_INT_FLAG_ADDR  & 0xFF) )  &&  ( pEEPROM->direciorn == DIR_TRANSMIT ) && (pEEPROM->DMA_TX == 1)  )
       {
    	 I2C_EnableDMA(I2C_NAME[ pEEPROM->dev ]);
    	// DMA_Enable(DMA1_Stream7);
   	     I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS2 );
   	     return;

     }
    if (( int_flags  & ((I2C_INT_FLAG_TXBE  | I2C_INT_FLAG_BTC)   & 0xFF) ) && (pEEPROM->DMA_TX == 0))
    {
    	pEEPROM->direciorn =   DIR_RECIEVE;
    	I2C_EnableGenerateStart( I2C_NAME[ pEEPROM->dev ] );
    	return;

    }
    if (( int_flags  & ((I2C_INT_FLAG_TXBE  | I2C_INT_FLAG_BTC)   & 0xFF) ) && (pEEPROM->DMA_TX == 1))
    {
    	I2C_EnableGenerateStop(  I2C_NAME[ pEEPROM->dev] );
        xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x01, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        return;

    }

#endif
}

void I2C_ERROR_FSM()
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	 uint16_t int_flags2 = I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS2 );
	 uint16_t int_flags =  I2C_ReadRegister(I2C_NAME[ pEEPROM->dev],  I2C_REGISTER_STS1 );
	 if ( ( int_flags & I2C_INT_FLAG_AE) == ( I2C_INT_FLAG_AE & 0xFFFF ) )
	 {
	    I2C_ClearIntFlag(I2C_NAME[ pEEPROM->dev],I2C_INT_FLAG_AE);
	    I2C_EnableGenerateStop( I2C_NAME[ pEEPROM->dev] );
	    xTaskNotifyIndexedFromISR(pEEPROM->NotifyTaskHeandle, pEEPROM->ucTaskNatificationIndex,0x02, eSetValueWithOverwrite, &xHigherPriorityTaskWoken  );
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	    return;
	 }

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

