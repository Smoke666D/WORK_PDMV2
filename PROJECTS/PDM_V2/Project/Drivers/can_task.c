/*
 * can_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "system_init.h"
#include "can_task.h"
#include "hw_lib_can.h"
#include "string.h"
#include "hw_lib_can.h"
#include "hal_can.h"

static CAN_ERROR_TYPE eCanError  		__section( TASK_RAM_SECTION ) ;
static QueueHandle_t pCanRXHandle    	__section( TASK_RAM_SECTION ) ;
static QueueHandle_t pCanTXHandle   	__section( TASK_RAM_SECTION ) ;
static uint16_t boundrate_can1 			__section( TASK_RAM_SECTION ) ;
static CANRX * MailBoxBuffer;
static TaskHandle_t  pCanRXTaskHandle  	__SECTION(RAM_SECTION_CCMRAM);
static TaskHandle_t  pCanTXTaskHandle  	__SECTION(RAM_SECTION_CCMRAM);

static void  prv_read_can_received_msg(  HAL_CAN_RX_FIFO_NUMBER_t fifo);
static void vReinit();
static void MessageIsSend();
/*
 *
 */
TaskHandle_t * xGetCanRXTaskHandle()
{
	return  &pCanRXTaskHandle ;
}
/*
 *
 */
TaskHandle_t * xGetCanTXTaskHandle()
{
	return  &pCanTXTaskHandle ;
}
/*
 *
*/
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
	uint8_t index =  (mailboxindex / 4 );
    HAL_CANSetFiters(index, MailBoxBuffer[index*4U  ].ident ,MailBoxBuffer[index*4U +1  ].ident,MailBoxBuffer[index*4U +2 ].ident,MailBoxBuffer[index*4U + 3 ].ident, FILTER_FIFO_0);
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
	vInitMailBoxBuffer();
	MailBoxBuffer = getMailBox();
	HAL_CANSetERRCallback( &vReinit);
	HAL_CANSetRXCallback(&prv_read_can_received_msg);
	HAL_CANSetTXCallback(&MessageIsSend);
	//xTaskNotify(pCanTXTaskHandle, 3, eSetValueWithOverwrite);
	return;
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
		xQueuePeek( pCanTXHandle, &TXPacket, portMAX_DELAY);
		if (HAL_CANSend(&TXPacket) != 3 )
		{
			xQueueReceive( pCanTXHandle, &TXPacket, 1);
			eCanError = CAN_NORMAL;
		}
	}
}




/*
 *
 */
void vCANBoudInit( uint16_t boudrate )
{

    HAL_CANToInitMode();
	boundrate_can1 = boudrate;
	HAL_CANIntIT( boundrate_can1,CAN_PRIOR,CAN_SUBPRIOR);
	HAL_CANToOperatingMode();

    return;
}

static void vReinit()
{


	//xQueueReset(pCanTXHandle);
	//xQueueReset(pCanRXHandle);
	//vCANBoudInit(boundrate_can1);
}





static void MessageIsSend()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//xTaskNotifyFromISR(pCanTXTaskHandle,0,eIncrement,&xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(pCanTXTaskHandle,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

static void  prv_read_can_received_msg(  HAL_CAN_RX_FIFO_NUMBER_t fifo)
{

    CAN_FRAME_TYPE rx_message;
    if (HAL_CANGetRXMessage(  fifo,  &rx_message ) ==  HAL_CAN_OK)
    	vCanInsertToRXQueue(& rx_message);
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



