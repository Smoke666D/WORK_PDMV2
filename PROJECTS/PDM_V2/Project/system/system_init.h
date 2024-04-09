/*
 * system_init.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef SYSTEM_SYSTEM_INIT_H_
#define SYSTEM_SYSTEM_INIT_H_

#include "main.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "event_groups.h"
#include "din_task.h"
#include "lua_task.h"

/*------------------------ Define --------------------------------------*/
#define  RAM_SECTION_RAM                ".ram"
#define  RAM_SECTION_CCMRAM             ".ccmram"
#define  TASK_RAM_SECTION                RAM_SECTION_CCMRAM

#define START_ACTION  0x01
#define STOP_ACTION   0x02
#define RESTAT_ACTION 0x03
#define READY_ACTION  0x04

#ifndef   __SECTION
  #define __SECTION(x) __attribute__((section(x)))
#endif

typedef enum
{
  OS_TASK_GROUPE_1,
  OS_TASK_GRPUPE_2,
  OS_TASK_GRPUPE_3
} OS_TASK_Groupe;

typedef enum
{
  TASK_IDLE_STATE = 0x00,
  TASK_INIT_STATE = 0x01,
  TASK_STOP_STATE = 0x02,
  TASK_PAUSE_STATE = 0x03,
  TASK_RUN_STATE = 0x04
} OS_TAKS_STATE;

#define TASK_RUN_NOTIFY  0x01
#define TASK_STOP_NOTIFY 0x02
#define TASK_INIT_NOTIFY 0x03
#define TASK_PAUSE_NOTIFY 0x03
#define  DIN_DRIVER_READY  0x04




void vSYStaskInit ( void );


#endif /* SYSTEM_SYSTEM_INIT_H_ */
