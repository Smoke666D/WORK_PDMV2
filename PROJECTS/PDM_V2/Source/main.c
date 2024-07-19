/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.3
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2021-2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "main.h"
#include "stdio.h"
#include "system_init.h"
#include "hw_lib_eeprom.h"
#include "can_task.h"
/** FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "hal_rtc.h"



void vInitGPIO()
{
	HAL_InitGPO();
	HAL_InitGpioAIN(PORT_A, GPIO_PIN_8 | GPIO_PIN_10  );
	HAL_InitGpioAIN(PORT_B, GPIO_PIN_12 | GPIO_PIN_13  | GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_2 );
	HAL_InitGpioAIN(PORT_C, GPIO_PIN_15| GPIO_PIN_13 | GPIO_PIN_14  );
	HAL_InitGpioAIN(PORT_D, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 );
	HAL_InitGpioAIN(PORT_G, GPIO_PIN_0  | GPIO_PIN_8 | GPIO_PIN_12| GPIO_PIN_13 | GPIO_PIN_14| GPIO_PIN_15);
	HAL_InitGpioAIN(PORT_E, GPIO_PIN_10 | GPIO_PIN_12 |GPIO_PIN_15 );
	HAL_InitGpioAIN(PORT_F, GPIO_PIN_2 | GPIO_PIN_11| GPIO_PIN_12);
	HAL_InitGpioAF(PORT_D, GPIO_PIN_0,  GPIO_AF_CAN1, MODE_OUT_PP );
	HAL_InitGpioAF(PORT_D, GPIO_PIN_1,  GPIO_AF_CAN1 ,MODE_OUT_PP);
	HAL_InitGpioAIN(CS20_1_GPIO_Port,CS20_1_Pin);
    HAL_InitGpioAIN(CS20_2_GPIO_Port,CS20_2_Pin);
	HAL_InitGpioAIN(CS20_3_GPIO_Port,CS20_3_Pin);
    HAL_InitGpioAIN(CS20_4_GPIO_Port,CS20_4_Pin);
    HAL_InitGpioAIN(CS20_5_GPIO_Port,CS20_5_Pin);
	HAL_InitGpioAIN(CS20_6_GPIO_Port,CS20_6_Pin);
    HAL_InitGpioAIN(CS20_7_GPIO_Port,CS20_7_Pin);
    HAL_InitGpioAIN(CS20_8_GPIO_Port,CS20_8_Pin);
	HAL_InitGpioAIN(CS8_9_GPIO_Port,CS8_9_Pin);
    HAL_InitGpioAIN(CS8_10_GPIO_Port,CS8_10_Pin);
	HAL_InitGpioAIN(CS8_11_GPIO_Port,CS8_11_Pin);
	HAL_InitGpioAIN(CS8_12_GPIO_Port,CS8_12_Pin);
	HAL_InitGpioAIN(CS8_13_GPIO_Port,CS8_13_Pin);
	HAL_InitGpioAIN(CS8_14_GPIO_Port,CS8_14_Pin);
	HAL_InitGpioAIN(CS8_15_GPIO_Port,CS8_15_Pin);
	HAL_InitGpioAIN(CS8_16_GPIO_Port,CS8_16_Pin);
    HAL_InitGpioAIN(CS8_17_GPIO_Port,CS8_17_Pin);
	HAL_InitGpioAIN(CS8_18_GPIO_Port,CS8_18_Pin);
	HAL_InitGpioAIN(CS8_19_GPIO_Port,CS8_19_Pin);
	HAL_InitGpioAIN(CS8_20_GPIO_Port,CS8_20_Pin);
	HAL_InitGpioAIN(ADC1_4_GPIO_Port,ADC1_4_Pin);
	HAL_InitGpioAIN(ADC1_5_GPIO_Port,ADC1_5_Pin);
    HAL_InitGpioAIN(ADC1_8_GPIO_Port,ADC1_8_Pin);
	HAL_InitGpioAIN(ADC1_9_GPIO_Port,ADC1_9_Pin);
}
/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 */
int main(void)
{
	//SystemCoreClockUpdate();
	NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_3);
	vInitGPIO();
    vRTCInit();
	vHWLilBaseTimerInit(TMR13);
	vSYSqueueInit ( );
	vSYSeventInit ( );
	vSYStaskInit ( );
    /* User create task */
	vCANinit();
	vInitEEPROM();
    /* Startup FreeRTOS */
    vTaskStartScheduler();
    while(1);
}





