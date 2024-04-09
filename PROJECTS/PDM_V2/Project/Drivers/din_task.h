/*
 * din_task.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVER_DIN_TASK_H_
#define DRIVER_DIN_TASK_H_

#include "main.h"
#include "system_init.h"
#include "hw_lib_din.h"
#include "hal_gpio.h"
#include "hal_timers.h"


#define DIN_TASK_STACK_SIZE  130
#define DIN_TASK_PRIORITY    2

typedef struct {
	uint16_t Pin;
	PortName_t PORT;
} PIN_CONFIG;

typedef enum  {
		CONFIG_OK = 0,
		CONFIG_NO_SUPPORT = 1U,
		WRONG_CHANNEL_NUMBER =2U,
		WRONG_CHANNEL_MODE =3U,
} PDM_INPUT_CONFIG_ERROR;

void NotifyTaskToStop(TaskHandle_t * pTask);
void NotifyTaskToInit(TaskHandle_t * pTask);
TaskHandle_t * xGetDinTaskHandle();
PDM_INPUT_CONFIG_ERROR eDinConfig( uint8_t ucCh, DIN_INPUT_TYPE inType, uint32_t ulHFront, uint32_t ulLFront);
void vDinTask(void *argument);
uint16_t uGetRPM1();
uint16_t uGetRPM2();


#endif /* DRIVER_DIN_TASK_H_ */
