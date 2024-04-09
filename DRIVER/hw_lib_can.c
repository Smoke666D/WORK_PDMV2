/*
 * hw_lib_can.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */



#include "hw_lib_can.h"
#include "string.h"
#include "FreeRTOS.h"
#include "event_groups.h"



void (*filter_set_function)(uint16_t);
CANRX MailBoxBuffer[MAILBOXSIZE] ;

CANRX * getMailBox()
{
	return MailBoxBuffer;
}


/*
 *
 */
void vCanInsertRXData(CAN_FRAME_TYPE * RXPacket)
{
	uint16_t id = RXPacket->filter_id;
	if (MailBoxBuffer[id].ident == RXPacket->ident)
	{
			MailBoxBuffer[id].DLC = RXPacket->DLC;
			for (int k =0;k <RXPacket->DLC;k++)
			{
				 MailBoxBuffer[id].data[k] =  RXPacket->data[k];
			}
			MailBoxBuffer[id].new_data = 1;
			return;
	}
	return;
}


void vsetFiterFunctionCallBack( void * t  )
{
	filter_set_function = t;

}

void vSetWaitFilter(uint32_t id)
{
	 MailBoxBuffer[0].ident = id;
	 filter_set_function(0);
	 return;
}





/*
 *
 */
uint8_t vCheckAnswer( void )
{
	 return ( MailBoxBuffer[0].new_data );
}
/*
 *
 */
ERROR_TYPE_t eMailboxFilterSet(uint32_t id)
{
	ERROR_TYPE_t eRes = BUFFER_FULL;
	for (int i=1;i<MAILBOXSIZE;i++)
	{
		if (MailBoxBuffer[i].ident == 0U)
		{
			 MailBoxBuffer[i].ident = id;
			 filter_set_function(i);
			 eRes = ERROR_NO;
			 break;
		}
	}
	return ( eRes );
}
/*
 *
 */
void vResetMailboxFilter(uint32_t id)
{
	for (int i=1;i<MAILBOXSIZE;i++)
	{
		if (MailBoxBuffer[i].ident == id)
		{
			 MailBoxBuffer[i].ident = 0U;
			 MailBoxBuffer[i].new_data = 0U;
			 break;
		}
	}
	return;
}
/*
 *
 */
void vInitMailBoxBuffer( void )
{
	for (int i=0;i<MAILBOXSIZE;i++)
	{
		 MailBoxBuffer[i].ident = 0U;
		 MailBoxBuffer[i].new_data = 0U;
	}
	return;
}




uint8_t vCanChekMessage(uint32_t id)
{
	uint8_t ucRes = 0;
	uint8_t max_data = MAILBOXSIZE;
	for (int k=0;k < max_data;k++)
	{
		if ((MailBoxBuffer[k].new_data == 1) && (MailBoxBuffer[k].ident = id))
		{
				ucRes = 1U;
				break;
		}
	}
	return ( ucRes );
}
/*
 *
 */
uint8_t vCanGetRequest(CAN_FRAME_TYPE * RXPacket)
{
	uint8_t res = 0U;
	res = vCheckAnswer();
	if ( res != 0U )
	{
		RXPacket->ident = MailBoxBuffer[0].ident;
		RXPacket->DLC = MailBoxBuffer[0].DLC;
		for (int i =0; i < RXPacket->DLC;i++)
		{
			RXPacket->data[i] = MailBoxBuffer[ 0U ].data[i];
		}
		MailBoxBuffer[0].new_data 	= 0U;
		MailBoxBuffer[0].ident 		= 0U;
	}
	return ( res );
}
/*
 *
 */
uint8_t vCanGetMessage(CAN_FRAME_TYPE * RXPacket)
{
	uint8_t res = 0U;
	for (int k=0;k < MAILBOXSIZE;k++)
	{
		if ((MailBoxBuffer[k].new_data == 1) && (MailBoxBuffer[k].ident == RXPacket->ident))
		{
			RXPacket->DLC = MailBoxBuffer[k].DLC;
			for (int i =0; i < RXPacket->DLC;i++)
			{
				RXPacket->data[i] = MailBoxBuffer[k].data[i];
			}
			MailBoxBuffer[k].new_data = 0;
			res = 1U;
			break;
		}
	}
	return ( res );
}
