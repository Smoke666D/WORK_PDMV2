/*
 * hw_lib_can.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HW_LIB_HW_LIB_CAN_H_
#define HW_LIB_HW_LIB_CAN_H_


#include "main.h"
#include "hal_can.h"

#define MAILBOXSIZE  14*4







typedef struct {
    uint16_t ident;
    uint16_t filter_id;
    uint8_t DLC;
    uint8_t data[8];
    uint8_t new_data;
} CANRX;



typedef enum {
	CAN_OFF		 	= 0,
	CAN_NORMAL	 	= 1,
	CAN_CUT_OFF 	= 2,
	CAN_SHORT_CUT 	= 3,

} CAN_ERROR_TYPE;

typedef enum
{
  ERROR_NO  = 0,
  BUFFER_FULL = 1
} ERROR_TYPE_t;

typedef enum {
  CAN_OK,
  CAN_NO_TX_MAILBOX,

} CAN_ERROR_t;

uint8_t vCanGetMessage(CAN_FRAME_TYPE * RXPacket);
uint8_t vCanGetRequest(CAN_FRAME_TYPE * RXPacket);
uint8_t vCheckAnswer( void );
uint8_t vCanChekMessage(uint32_t id);
ERROR_TYPE_t eMailboxFilterSet(uint32_t id);
void vSetWaitFilter(uint32_t id);
void vSetWaitFilter(uint32_t id);
void vsetFiterFunctionCallBack( void * t  );
CANRX * getMailBox();
void vInitMailBoxBuffer( void );
void vCanInsertRXData(CAN_FRAME_TYPE * RXPacket);

#endif /* HW_LIB_HW_LIB_CAN_H_ */
