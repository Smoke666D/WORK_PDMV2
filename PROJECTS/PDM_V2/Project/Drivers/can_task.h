/*
 * can_task.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_CAN_TASK_H_
#define DRIVERS_CAN_TASK_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"


#define CANT_TX0_FREE   0x0001
#define CANT_TX1_FREE   0x0002
#define CANT_TX2_FREE   0x0004

void vCANBoudInit( uint16_t boudrate );
void vCANinit();
void vCanInsertTXData(uint32_t CanID, uint8_t * data, uint8_t data_len );
EventGroupHandle_t * pCANEventGet();
QueueHandle_t* pCANRXgetQueue ( void );
QueueHandle_t* pCANTXgetQueue ( void );
void vCanRXTask(void *argument);
void vCanTXTask(void *argument);

#endif /* DRIVER_CAN_TASK_H_ */
