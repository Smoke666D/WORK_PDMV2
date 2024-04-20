/*
 * lua_task.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */
#ifndef DRIVERS_LUA_TASK_H_
#define DRIVERS_LUA_TASK_H_

#include "main.h"
#include "system_init.h"


#define LUA_TASK_PRIORITY    2



//#include "system.h"
#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"


#define PDM_STOP_STATE   0x01
#define PDM_RUN_STATE   0x02
#define PDM_INIT_STATE   0x04
#define PDM_BUSY_STATE  0x08

#define RUN_STATE 	   0x01
#define STOPR_STATE    0x02
#define RESTART_STATE  0x04
#define CAN_ERROR      0x08
#define DIN_RESTART    0x10

typedef enum {
	LUA_INIT,
	LUA_RUN,
	LUA_ERROR,
	LUA_STOP,
	LUA_RESTART,
	LUA_STATE_SIZE,
	LUA_WAIT_READY
} LUA_STATE_t;


typedef enum {
	IS_ENABLE   = 0,
	IS_DISABLE  = 1
} ENABLE_t;


typedef enum
{
	RESULT_TRUE = 1,
	RESULT_FALSE =0
} RESULT_t;

void vLUAInitPDM();
const char * pcGetLUAError ( void );
uint32_t ulLUAgetWorkCicle ( void );
LUA_STATE_t eLUAgetSTATE ( void );
void vLUAstopPDM();
void vLUArestartPDM();
EventGroupHandle_t* osLUAetPDMstatusHandle ( void );
TaskHandle_t * xGetLUATaskHandle();
void vLuaTask(void *argument);

#endif /* DRIVERS_LUA_TASK_H_ */
