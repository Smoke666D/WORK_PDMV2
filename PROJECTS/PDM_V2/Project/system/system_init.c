/*
 * system_init.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "system_init.h"
#include "hw_lib_can.h"
#include "hw_lib_eeprom.h"
#include "can_task.h"
#include "lua_task.h"





#define TASK_COUNT 2

/*
 * Стеки и дискрипторы для статичесокго размещения IDLE task и таймера.
 */
static StaticTask_t xIdleTaskTCB  									 __SECTION(RAM_SECTION_CCMRAM);
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ] 		 __SECTION(RAM_SECTION_CCMRAM);
static StaticTask_t xTimerTaskTCB 									 __SECTION(RAM_SECTION_CCMRAM);
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ]  __SECTION(RAM_SECTION_CCMRAM);
/*
 * Переменные
 */

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{



   /* Передача наружу указателя на структуру StaticTask_t, в которой
      задача Idle будет сохранять свое состояние. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
   /* Передача наружу размера массива, на который указывает *ppxIdleTaskStackBuffer.
      Обратите внимание, что поскольку массив должен быть типа StackType_t, то
      configMINIMAL_STACK_SIZE указывается в словах, а не в байтах. */
   *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* Оба параметра configSUPPORT_STATIC_ALLOCATION и configUSE_TIMERS установлены
   в 1, поэтому приложение должно предоставить реализацию vApplicationGetTimerTaskMemory()
   чтобы обеспечить памятью задачу Timer service. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
   /* Если буферы, предоставленные для Timer task, декларированы внутри этой
      функции, то они должны быть с атрибутом static – иначе буферы будут
      уничтожены при выходе из этой функции. */

   /* Передача наружу указателя на структуру StaticTask_t, в которой будет
      сохраняться состояние задачи Timer service. */
   *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

   /* Передача наружу массива, который будет использоваться как стек
      задачи Timer service. */
   *ppxTimerTaskStackBuffer = uxTimerTaskStack;

   /* Передача наружу размера массива, на который указывает *ppxTimerTaskStackBuffer.
      Обратите внимание, что поскольку массив должен быть типа StackType_t, то
      configTIMER_TASK_STACK_DEPTH указывается в словах, а не в байтах. */
   *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}



static TaskHandle_t defaultTaskHandle __SECTION(RAM_SECTION_CCMRAM);
/*
 * Функции
*/
static void StartDefaultTask(void *argument);



static StackType_t usbTaskBuffer[USB_TASK_STACK_SIZE]           __section( TASK_RAM_SECTION );
static StackType_t luaTaskBuffer[LUA_TASK_STACK_SIZE]          	__section( TASK_RAM_SECTION );
static StackType_t dinTaskBuffer[DIN_TASK_STACK_SIZE]           __section( TASK_RAM_SECTION );
static StackType_t adcTaskBuffer[ADC_TASK_STACK_SIZE]           __section( TASK_RAM_SECTION );
static StackType_t canTXTaskBuffer[CAN_TX_TASK_STACK_SIZE]      __section( TASK_RAM_SECTION );
static StackType_t canRXTaskBuffer[CAN_RX_TASK_STACK_SIZE]      __section( TASK_RAM_SECTION );

static StaticTask_t usbTaskControlBlock          				__section( TASK_RAM_SECTION );
static StaticTask_t luaTaskControlBlock          			    __section( TASK_RAM_SECTION );
static StaticTask_t dinTaskControlBlock           				__section( TASK_RAM_SECTION );
static StaticTask_t adcTaskControlBlock           				__section( TASK_RAM_SECTION );
static StaticTask_t canTXTaskControlBlock            			__section( TASK_RAM_SECTION );
static StaticTask_t canRXTaskControlBlock            			__section( TASK_RAM_SECTION );

static StaticEventGroup_t xLuaStateEventGroup 					__section( TASK_RAM_SECTION );


void vSYStaskInit ( void )
{

//xTaskCreate(StartDefaultTask, "Task2",  128, NULL, 0 ,&defaultTaskHandle);

  (* xGetADCTaskHandle()) =
  xTaskCreateStatic( vADCTask, "ADCTask", ADC_TASK_STACK_SIZE, ( void * ) 1, 12, (StackType_t * const )adcTaskBuffer, &adcTaskControlBlock );
  (* xGetDinTaskHandle())  =
  xTaskCreateStatic( vDinTask, "DinTask", DIN_TASK_STACK_SIZE, ( void * ) 1, 11, (StackType_t * const )dinTaskBuffer,  &dinTaskControlBlock );
  (* xGetLUATaskHandle()) =
  xTaskCreateStatic( vLuaTask, "LuaTask", LUA_TASK_STACK_SIZE, ( void * ) 1, 9, (StackType_t * const )luaTaskBuffer, &luaTaskControlBlock );
  (* osUSBgetTaskHandle()) =
		  xTaskCreateStatic(
				  	  	  	  	vUSBtask,       /* Function that implements the task. */
		  					   "USBTask",          /* Text name for the task. */
							   USB_TASK_STACK_SIZE ,      /* Number of indexes in the xStack array. */
		                         ( void * ) 1,    /* Parameter passed into the task. */
		                         10,/* Priority at which the task is created. */
		  					   (StackType_t * const )usbTaskBuffer,          /* Array to use as the task's stack. */
		                         &usbTaskControlBlock);  /* Variable to hold the task's data structure. */

  (* xGetCanRXTaskHandle()) =
  xTaskCreateStatic( vCanRXTask, "CanRxTask", CAN_RX_TASK_STACK_SIZE , ( void * ) 1, 8, (StackType_t * const )canRXTaskBuffer, &canRXTaskControlBlock );
  (* xGetCanTXTaskHandle()) =
  xTaskCreateStatic( vCanTXTask, "CanTxTask", CAN_TX_TASK_STACK_SIZE , ( void * ) 1, 8, (StackType_t * const )canTXTaskBuffer, &canTXTaskControlBlock );
  return;
}

void vSYSqueueInit ( void )
{
  *( pCANRXgetQueue() ) = xQueueCreate( CANRX_QUEUE_SIZE, sizeof( CAN_FRAME_TYPE));
  *( pCANTXgetQueue() ) = xQueueCreate( CANTX_QUEUE_SIZE, sizeof( CAN_TX_FRAME_TYPE ) );
 // *(  pEEPROMgetQueue ())  =xQueueCreate( EEPROMTX_QUEUE_SIZE, sizeof( EEPROM_SECTOR_t ) );
//  *(  pEEPROMgetRXQueue ())  =xQueueCreate( 1, sizeof( EEPROM_SECTOR_t ) );
}
/*----------------------------------------------------------------------------*/
void vSYSeventInit ( void )
{
  *(osLUAetPDMstatusHandle () ) = xEventGroupCreateStatic(&xLuaStateEventGroup );
 // *(pCANEventGet())				= xEventGroupCreate( );
 // *(osEEPROMstatusHandle () )    = xEventGroupCreate( );
}

static void StartDefaultTask(void *argument)
{
	//DinNotifyTaskToInit(&defaultTaskHandle);
	//AinNotifyTaskToInit(&defaultTaskHandle);
	 while(1)
	 {
	        /* Task blocking time Delay */
		   // WDT_Reset();
	        vTaskDelay(100);
	 }

}
