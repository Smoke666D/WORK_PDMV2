/*
 * din_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "din_task.h"

const  PIN_CONFIG xDinPortConfig[]           = {{Din1_Pin,Din1_Port},
											    {Din2_Pin,Din2_Port},
											    {Din3_Pin,Din3_Port},
											    {Din4_Pin,Din4_Port},
											    {Din5_Pin,Din5_Port},
											    {Din6_Pin,Din6_Port},
											    {Din7_Pin,Din7_Port},
											    {Din8_Pin,Din8_Port},
											    {Din9_Pin,Din9_Port},
											    {Din10_Pin,Din10_Port},
											    {Din11_Pin,Din11_Port},
											    {Din12_Pin,Din12_Port},

};
/*
 *  Статические переменные объкта
 */
static TaskHandle_t  pTaskHandle  			__SECTION(RAM_SECTION_CCMRAM);


/*
 *  Функции управления процессом
 */
TaskHandle_t * xGetDinTaskHandle()
{
	return  &pTaskHandle;
}


void DinNotifyTaskToStop()
{
	ulTaskNotifyValueClear(pTaskHandle, TASK_RUN_NOTIFY);
}

void DinNotifyTaskToInit()
{

	xTaskNotify(pTaskHandle, TASK_RUN_NOTIFY , eSetValueWithOverwrite);
}



FLAG_t fDinState (uint8_t i)
{
	if (i <= INPUT_12 )
		return (FLAG_t)HAL_GetBit( xDinPortConfig[i].PORT, xDinPortConfig[i].Pin);
	else
       return (RESET);
}
/*
 *
 */
uint16_t uGetRPM1()
{
	return (  GetRPM(INPUT_9 ) );
}
/*
 *
 */
uint16_t uGetRPM2()
{
	return (  GetRPM(INPUT_6) );
}


#define CHECK_DIN_INIT_STATE_COUNT 3
/*
 *
 */
PDM_INPUT_CONFIG_ERROR eDinConfig( uint8_t ucCh, DIN_INPUT_TYPE inType, uint32_t ulHFront, uint32_t ulLFront)
{
	PDM_INPUT_CONFIG_ERROR eRes = WRONG_CHANNEL_NUMBER;
	DinConfig_t DIN_CONFIG;
	if ( ucCh < DIN_COUNT)
	{
		if (inType != RPM_CONFIG)
		{
			DIN_CONFIG.eInputType = inType;
			DIN_CONFIG.ulHighCounter = ulLFront;
			DIN_CONFIG.ulLowCounter  = ulHFront;
			DIN_CONFIG.getPortCallback = &fDinState;

			if (( xGetDinType(ucCh) == DIN_IDLE) || (xGetDinType(ucCh)== RPM_CONFIG))
			{
				eDinConfigWtihStruct(ucCh,&DIN_CONFIG);
				HAL_InitGpioInPUP(xDinPortConfig[ucCh].PORT, xDinPortConfig[ucCh].Pin);

			}
			else
				vRecinfigDin(ucCh,&DIN_CONFIG);
			if (  ucCh == INPUT_9 ) HAL_TiemrDisable(TIMER10);
			if (  ucCh == INPUT_6 ) HAL_TiemrDisable(TIMER9);
			eRes = CONFIG_OK;

		}
		else
		{
			if ( ( ucCh == INPUT_9 ) || ( ucCh == INPUT_6 ) )
			{
				HAL_InitGpioAF(xDinPortConfig[ucCh].PORT, xDinPortConfig[ucCh].Pin, (ucCh == INPUT_9) ? TIMER10_AF  : TIMER9_AF , MODE_OUT_PP);
			    if (ucCh == INPUT_9)
			    {
			    	eRPMConfig(INPUT_9 ,RPM_CH1);
			    	HAL_InitCaptureIRQTimer( TIMER10 , 1000, 60000, TIM_CHANNEL_1);
			    }
			    else
			    {
			    	eRPMConfig(INPUT_6 ,RPM_CH2);
			    	HAL_InitCaptureIRQTimer( TIMER9 , 1000, 60000, TIM_CHANNEL_1);
			    }
				eRes = CONFIG_OK;
			}
		}
	}
	return ( eRes );
}



void TMR1_BRK_TMR9_IRQHandler(void)
{
	uint16_t temp_data = vHAL_CaptureTimerInteruptCallback(TIMER9 , TIM_CC1_FLAG  );
	if (temp_data > EDGE )
		vAddRPMData(INPUT_9, temp_data );
	return;

}
void TMR1_UP_TMR10_IRQHandler(void)
{
	uint16_t temp_data = vHAL_CaptureTimerInteruptCallback(TIMER10 , TIM_CC1_FLAG  );
	if (temp_data > EDGE )
		vAddRPMData(INPUT_6, temp_data );
	return;
}


void vDINInit()
{
	 InitFilters();
	 eDinConfig( INPUT_1, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_2, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_3, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_4, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_5, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_6, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_7, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_8, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_9, DIN_CONFIG_POSITIVE , DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_10, DIN_CONFIG_POSITIVE ,DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_11, DIN_CONFIG_POSITIVE ,DEF_H_FRONT, DEF_L_FRONT );
	 eDinConfig( INPUT_12, DIN_CONFIG_POSITIVE ,DEF_H_FRONT, DEF_L_FRONT );
}


void vDinTask(void *argument)
{
    uint32_t ulNotifiedValue;
    InitDinStcurt();
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 1 );
    xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		xTaskNotifyWait(0,0x00,&ulNotifiedValue,0);
		if ( ulNotifiedValue == TASK_RUN_NOTIFY )
		{
			vTaskDelayUntil( &xLastWakeTime, xPeriod );
			vDinDoutProcess();
		}
		else
		{
			xTaskNotifyWait(0,0x00,&ulNotifiedValue,portMAX_DELAY);
			vDinInitStateProcess();
		}


	}
}
