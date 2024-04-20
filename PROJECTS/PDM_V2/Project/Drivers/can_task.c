/*
 * can_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "system_init.h"
#include "can_task.h"
#include "hw_lib_can.h"
#include "apm32f4xx_can.h"
#include "apm32f4xx_rcm.h"
#include "string.h"
#include "hw_lib_can.h"
#include "apm32f4xx.h"

static CAN_ERROR_TYPE eCanError  	__section( TASK_RAM_SECTION ) ;
QueueHandle_t pCanRXHandle    		__section( TASK_RAM_SECTION ) ;
QueueHandle_t pCanTXHandle   		__section( TASK_RAM_SECTION ) ;
//EventGroupHandle_t xCANstatusEvent  __section( TASK_RAM_SECTION ) ;
static CANRX * MailBoxBuffer;
static TaskHandle_t  pCanRXTaskHandle  	__SECTION(RAM_SECTION_CCMRAM);
static TaskHandle_t  pCanTXTaskHandle  	__SECTION(RAM_SECTION_CCMRAM);

TaskHandle_t * xGetCanRXTaskHandle()
{
	return  &pCanRXTaskHandle ;
}
TaskHandle_t * xGetCanTXTaskHandle()
{
	return  &pCanTXTaskHandle ;
}


static void vCANHWInit()
{

	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_AF;
	gpioConfigStruct.pin = GPIO_PIN_0|GPIO_PIN_1;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_100MHz  ;
	GPIO_Config(GPIOG, &gpioConfigStruct);

	GPIO_ConfigPinAF(GPIOG, GPIO_PIN_SOURCE_0, GPIO_AF_CAN1);
	GPIO_ConfigPinAF(GPIOG, GPIO_PIN_SOURCE_1, GPIO_AF_CAN1);

}

/*EventGroupHandle_t * pCANEventGet()
{

	return (&xCANstatusEvent);
}*/


QueueHandle_t* pCANRXgetQueue ( void )
{
  return ( &pCanRXHandle );
}
/*
 *
 */
QueueHandle_t* pCANTXgetQueue ( void )
{
  return ( &pCanTXHandle );
}

void vCanRXTask(void *argument)
{
	CAN_FRAME_TYPE RXPacket;
	while(1)
	{
		xQueueReceive( pCanRXHandle, &RXPacket,  portMAX_DELAY );
		vCanInsertRXData(&RXPacket);
		eCanError = CAN_NORMAL;
	}
}

void vFilterSet(uint16_t mailboxindex)
{
	 CAN_FilterConfig_T  sFilterConfig;
	 uint16_t index = mailboxindex / 4;
	 sFilterConfig.filterActivation = ENABLE;
     sFilterConfig.filterNumber = index;
     sFilterConfig.filterFIFO  = CAN_FILTER_FIFO_0 ;
     sFilterConfig.filterIdHigh 	= (MailBoxBuffer[index*4U +2U ].ident) <<5U ;
     sFilterConfig.filterIdLow  	= (MailBoxBuffer[index*4U  ].ident)    <<5U ;
     sFilterConfig.filterMaskIdHigh = (MailBoxBuffer[index*4U +3U ].ident) <<5U ;
     sFilterConfig.filterMaskIdLow  = (MailBoxBuffer[index*4U +1U ].ident) <<5U ;
     sFilterConfig.filterMode = CAN_FILTER_MODE_IDLIST;
     sFilterConfig.filterScale =CAN_FILTER_SCALE_16BIT;
     CAN_ConfigFilter(& sFilterConfig);
     return;
}


/*
 *
 */
void vCanInsertToRXQueue(CAN_FRAME_TYPE * data)
{
	static portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendFromISR( pCanRXHandle, data, &xHigherPriorityTaskWoken );
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	return;
}

void vCANinit()
{
	eCanError = CAN_OFF;
	vCANHWInit();
	vInitMailBoxBuffer();
	MailBoxBuffer = getMailBox();
	//xEventGroupSetBits(xCANstatusEvent, CANT_TX0_FREE | CANT_TX1_FREE |CANT_TX2_FREE);
	xTaskNotify(pCanTXTaskHandle, 3, eSetValueWithOverwrite);
	return;
}


uint8_t uPDMCanSend(CAN_TX_FRAME_TYPE *buffer)
{
   CAN_TxMessage_T pTXHeader;
   pTXHeader.dataLengthCode               = (uint32_t)buffer->DLC;
   if ( buffer->ident & CAN_EXT_FLAG)
   {
    	buffer->ident 		&= ~CAN_EXT_FLAG;
    	pTXHeader.extID      =  buffer->ident;
    	pTXHeader.typeID  = CAN_TYPEID_EXT;
    	pTXHeader.remoteTxReq        = (buffer->ident & FLAG_RTR) ? CAN_RTXR_REMOTE : CAN_RTXR_DATA;
    	pTXHeader.stdID 	 = 0U;
    }
    else
    {
    	pTXHeader.extID         = 0U;
    	pTXHeader.typeID        = CAN_TYPEID_STD;
    	pTXHeader.remoteTxReq   = (buffer->ident & FLAG_RTR) ? CAN_RTXR_REMOTE : CAN_RTXR_DATA;
        pTXHeader.stdID         = buffer->ident & CANID_MASK;

    }
   return CAN_TxMessage(CAN1, &pTXHeader);

}


/*
 * Процесс для обработки can сообщений
 */
void vCanTXTask(void *argument)
{
	CAN_TX_FRAME_TYPE TXPacket;
	uint8_t mailboxnumber;
	while(1)
	{
		//xEventGroupWaitBits(xCANstatusEvent, CANT_TX0_FREE | CANT_TX1_FREE | CANT_TX2_FREE , pdFALSE, pdFALSE, portMAX_DELAY );
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		xQueuePeek( pCanTXHandle, &TXPacket, portMAX_DELAY);
		mailboxnumber = uPDMCanSend(&TXPacket);
		if (mailboxnumber!=3)
		{
			xQueueReceive( pCanTXHandle, &TXPacket, 1);
			eCanError = CAN_NORMAL;
			/*switch (mailboxnumber)
			{
				case 0:
					xEventGroupClearBits(xCANstatusEvent, CANT_TX0_FREE );
					break;
				case 1:
					xEventGroupClearBits(xCANstatusEvent, CANT_TX1_FREE);
					break;
				case 2:
					xEventGroupClearBits(xCANstatusEvent, CANT_TX2_FREE);
					break;
			}*/
		}
	}
}


void CO_CANmodule_init(  uint16_t   CANbitRate)
{
	CAN_Config_T       CAN_ConfigStructure;

	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1 );


    //Конфигурация драйвера
	CAN_ConfigStructure.mode             = CAN_MODE_NORMAL;
	CAN_ConfigStructure.autoBusOffManage = ENABLE;
	CAN_ConfigStructure.autoBusOffManage = DISABLE;
	CAN_ConfigStructure.autoWakeUpMode   = DISABLE;
	CAN_ConfigStructure.nonAutoRetran    = ENABLE;
	CAN_ConfigStructure.rxFIFOLockMode   = DISABLE;
	CAN_ConfigStructure.txFIFOPriority   = DISABLE;

    /* Configure CAN timing */
      switch (CANbitRate)
      {
            case 1000:  CAN_ConfigStructure.prescaler = 2;
                   break;
            case 500:  CAN_ConfigStructure.prescaler = 4;
                   break;
            case 250:  CAN_ConfigStructure.prescaler = 8;
                   break;
            default:
            case 125:  CAN_ConfigStructure.prescaler = 16;
                  break;
            case 100:  CAN_ConfigStructure.prescaler = 20;
                 break;
            case 50:  CAN_ConfigStructure.prescaler = 120;
                 break;
            case 20:  CAN_ConfigStructure.prescaler = 300;
                 break;
            case 10:  CAN_ConfigStructure.prescaler = 600;
                 break;
      }
      CAN_ConfigStructure.syncJumpWidth =  CAN_SJW_1;
      CAN_ConfigStructure.timeSegment1 = CAN_TIME_SEGMENT1_15;
      CAN_ConfigStructure.timeSegment2 = CAN_TIME_SEGMENT2_5;
      CAN_Config(CAN1, &CAN_ConfigStructure);

     CAN_EnableInterrupt(CAN1, CAN_INT_TXME | CAN_INT_BOF | CAN_INT_F0MP | CAN_INT_F1MP);
     NVIC_EnableIRQRequest(CAN1_RX0_IRQn, 5, 0);
     NVIC_EnableIRQRequest(CAN1_RX1_IRQn, 5, 0);
     NVIC_EnableIRQRequest(CAN1_SCE_IRQn, 5, 0);

}

static uint16_t boundrate_can1;
/*
 *
 */
void vCANBoudInit( uint16_t boudrate )
{

	CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_INIT);
	boundrate_can1 = boudrate;
	CO_CANmodule_init( boundrate_can1);
	CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_NORMAL);
    return;
}

void vReinit()
{
	xQueueReset(pCanTXHandle);
	xQueueReset(pCanRXHandle);
	vCANBoudInit(boundrate_can1);

}


void CAN1_TX_IRQHandler (void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC0) == SET )
	    {
		  CAN_ClearStatusFlag (CAN1, CAN_FLAG_REQC0);
		 // xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX0_FREE, &xHigherPriorityTaskWoken );
	    }
	    if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC1) == SET )
	    {
	    	CAN_ClearStatusFlag(CAN1, CAN_FLAG_REQC1);
	    	//xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX1_FREE, &xHigherPriorityTaskWoken );
	    }
	    if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC2) == SET )
	    {
	    	CAN_ClearStatusFlag (CAN1,CAN_FLAG_REQC2);
	    	//xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX2_FREE, &xHigherPriorityTaskWoken );
	    }
	    CAN_ClearIntFlag(CAN1, CAN_INT_TXME );
	    vTaskNotifyGiveFromISR(pCanTXTaskHandle,&xHigherPriorityTaskWoken);
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}


static void  prv_read_can_received_msg( CAN_RX_FIFO_T fifo)
{
    CAN_RxMessage_T rx;
    CAN_FRAME_TYPE rx_message;
    CAN_RxMessage(CAN1, fifo, &rx) ;

    //Игнорируем RTR фрайм, поскольку у него нет данных для нас.
    if ( rx.remoteTxReq != CAN_RTXR_REMOTE )
    {
    	rx_message.ident = rx.stdID;
    	rx_message.DLC = rx.dataLengthCode;
    	rx_message.filter_id = rx.filterMatchIndex;
    	memcpy(rx_message.data,rx.data, 8 );
    	vCanInsertToRXQueue(& rx_message);
    }
    return;
}


/*
 *
 */
void vCanInsertTXData(uint32_t CanID, uint8_t * data, uint8_t data_len )
{
	CAN_TX_FRAME_TYPE data_to_send;
	data_to_send.ident = CanID;
	data_to_send.DLC   = data_len;
	for (uint8_t i=0; i<data_len;i++)
	{
		data_to_send.data[i] = data[i];
	}
	xQueueSend(pCanTXHandle, &data_to_send, 1U);
	return;
}

void  CAN1_RX0_IRQHandler (void )
{
	prv_read_can_received_msg( CAN_RX_FIFO_0);
	CAN_ClearIntFlag(CAN1, CAN_INT_F0MP );
}
void CAN1_RX1_IRQHandler(void)
{
	prv_read_can_received_msg( CAN_RX_FIFO_1);
	CAN_ClearIntFlag(CAN1,  CAN_INT_F1MP );
}
void CAN1_SCE_IRQHandler(void)
{
	CAN_ClearIntFlag(CAN1, CAN_INT_BOF );
	vReinit();

}
