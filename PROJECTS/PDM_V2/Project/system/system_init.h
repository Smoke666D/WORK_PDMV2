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
#include "usb_task.h"
#include "adc_task.h"


#define  DEFAULT_TASK_STACK_SIZE        128U
#define  LUA_TASK_STACK_SIZE            3000U
#define  ADC_TASK_STACK_SIZE            250U
#define  DOUT_TASK_STACK_SIZE           128U
#define  CAN_RX_TASK_STACK_SIZE         128U
#define  CAN_TX_TASK_STACK_SIZE         128U
#define  DEFAULT_TASK_STACK_SIZE         128U
#define  DIN_TASK_STACK_SIZE            128U
#define  SERIAL_TX_TSAK_STACK_SIZE      256U
#define  SERIAL_RX_TSAK_STACK_SIZE      128U
#define  SERIAL_PROTECT_TSAK_STACK_SIZE 128U
#define  USB_TASK_STACK_SIZE            192U


typedef enum {
  osPriorityNone          =  0,         ///< No priority (not initialized).
  osPriorityIdle          =  1,         ///< Reserved for Idle thread.
  osPriorityLow           =  8,         ///< Priority: low
  osPriorityLow1          =  8+1,       ///< Priority: low + 1
  osPriorityLow2          =  8+2,       ///< Priority: low + 2
  osPriorityLow3          =  8+3,       ///< Priority: low + 3
  osPriorityLow4          =  8+4,       ///< Priority: low + 4
  osPriorityLow5          =  8+5,       ///< Priority: low + 5
  osPriorityLow6          =  8+6,       ///< Priority: low + 6
  osPriorityLow7          =  8+7,       ///< Priority: low + 7
  osPriorityBelowNormal   = 16,         ///< Priority: below normal
  osPriorityBelowNormal1  = 16+1,       ///< Priority: below normal + 1
  osPriorityBelowNormal2  = 16+2,       ///< Priority: below normal + 2
  osPriorityBelowNormal3  = 16+3,       ///< Priority: below normal + 3
  osPriorityBelowNormal4  = 16+4,       ///< Priority: below normal + 4
  osPriorityBelowNormal5  = 16+5,       ///< Priority: below normal + 5
  osPriorityBelowNormal6  = 16+6,       ///< Priority: below normal + 6
  osPriorityBelowNormal7  = 16+7,       ///< Priority: below normal + 7
  osPriorityNormal        = 24,         ///< Priority: normal
  osPriorityNormal1       = 24+1,       ///< Priority: normal + 1
  osPriorityNormal2       = 24+2,       ///< Priority: normal + 2
  osPriorityNormal3       = 24+3,       ///< Priority: normal + 3
  osPriorityNormal4       = 24+4,       ///< Priority: normal + 4
  osPriorityNormal5       = 24+5,       ///< Priority: normal + 5
  osPriorityNormal6       = 24+6,       ///< Priority: normal + 6
  osPriorityNormal7       = 24+7,       ///< Priority: normal + 7
  osPriorityAboveNormal   = 32,         ///< Priority: above normal
  osPriorityAboveNormal1  = 32+1,       ///< Priority: above normal + 1
  osPriorityAboveNormal2  = 32+2,       ///< Priority: above normal + 2
  osPriorityAboveNormal3  = 32+3,       ///< Priority: above normal + 3
  osPriorityAboveNormal4  = 32+4,       ///< Priority: above normal + 4
  osPriorityAboveNormal5  = 32+5,       ///< Priority: above normal + 5
  osPriorityAboveNormal6  = 32+6,       ///< Priority: above normal + 6
  osPriorityAboveNormal7  = 32+7,       ///< Priority: above normal + 7
  osPriorityHigh          = 40,         ///< Priority: high
  osPriorityHigh1         = 40+1,       ///< Priority: high + 1
  osPriorityHigh2         = 40+2,       ///< Priority: high + 2
  osPriorityHigh3         = 40+3,       ///< Priority: high + 3
  osPriorityHigh4         = 40+4,       ///< Priority: high + 4
  osPriorityHigh5         = 40+5,       ///< Priority: high + 5
  osPriorityHigh6         = 40+6,       ///< Priority: high + 6
  osPriorityHigh7         = 40+7,       ///< Priority: high + 7
  osPriorityRealtime      = 48,         ///< Priority: realtime
  osPriorityRealtime1     = 48+1,       ///< Priority: realtime + 1
  osPriorityRealtime2     = 48+2,       ///< Priority: realtime + 2
  osPriorityRealtime3     = 48+3,       ///< Priority: realtime + 3
  osPriorityRealtime4     = 48+4,       ///< Priority: realtime + 4
  osPriorityRealtime5     = 48+5,       ///< Priority: realtime + 5
  osPriorityRealtime6     = 48+6,       ///< Priority: realtime + 6
  osPriorityRealtime7     = 48+7,       ///< Priority: realtime + 7
  osPriorityISR           = 56,         ///< Reserved for ISR deferred thread.
  osPriorityError         = -1,         ///< System cannot determine priority or illegal priority.
  osPriorityReserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osPriority_t;
/*------------------------ Define --------------------------------------*/
#define  RAM_SECTION_RAM                ".ram"
#define  RAM_SECTION_CCMRAM             ".ccmram"
#define  TASK_RAM_SECTION                RAM_SECTION_CCMRAM

#define START_ACTION  0x01
#define STOP_ACTION   0x02
#define RESTAT_ACTION 0x03
#define READY_ACTION  0x04

#define  SERIAL_QUEUE_SIZE              16U
#define  CANRX_QUEUE_SIZE 				16U
#define  CANTX_QUEUE_SIZE 				16U

#ifndef   __SECTION
  #define __SECTION(x) __attribute__((section(x)))
#endif



typedef enum
{
  TASK_IDLE_STATE = 0x00,
  TASK_INIT_STATE = 0x01,
  TASK_STOP_STATE = 0x02,
  TASK_PAUSE_STATE = 0x03,
  TASK_RUN_STATE = 0x04
} OS_TAKS_STATE;

#define TASK_RUN_NOTIFY    0x01
#define TASK_STOP_NOTIFY   0x02
#define TASK_INIT_NOTIFY   0x04
#define DIN_DRIVER_READY   0x08
#define AIN_DRIVER_READY   0x10
#define ADC2_READY         0x20
#define ADC3_READY         0x40
#define ADC1_READY         0x80

#define CAN_PRIOR         5
#define CAN_SUBPRIOR      0
#define ADC_DMA_PRIOR     6
#define ADC_DMA_SUBPRIOR  0
#define CAP_TIM_PRIOR     5
#define CAP_TIM_SUBPRIOR  1
#define I2C_PRIOR         4
#define I2C_SUBPRIOR      0//


void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize );

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize );

void vSYStaskInit ( void );
void vSYSqueueInit ( void );
void vSYSeventInit ( void );

#endif /* SYSTEM_SYSTEM_INIT_H_ */
