/*
 * system_init.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "system_init.h"


#define TASK_COUNT 2
/*
 * Переменные
 */

static TaskHandle_t defaultTaskHandle;
/*
 * Функции
*/
static void StartDefaultTask(void *argument);






void vSYStaskInit ( void )
{

 xTaskCreate(vDinTask, "DinTask",  DIN_TASK_STACK_SIZE, NULL, DIN_TASK_PRIORITY  ,xGetDinTaskHandle());
 xTaskCreate(StartDefaultTask, "Task2",  128, NULL, 2 ,&defaultTaskHandle);
 return;
}



static void StartDefaultTask(void *argument)
{
	 while(1)
	 {
	        /* Task blocking time Delay */
		   // WDT_Reset();
	        vTaskDelay(100);
	 }

}
