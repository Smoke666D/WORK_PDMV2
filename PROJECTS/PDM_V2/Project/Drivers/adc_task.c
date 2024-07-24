/*
 * adc_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "adc_task.h"
#include "lua_task.h"
#include "hal_adc.h"
#include "drivers_config.h"
#include "hal_dma.h"
#define TEMP_DATA    5


static PDM_OUTPUT_TYPE out[OUT_COUNT]  	 				__SECTION(RAM_SECTION_CCMRAM);
static TaskHandle_t  pTaskHandle  						__SECTION(RAM_SECTION_CCMRAM);
static float total_current __SECTION(RAM_SECTION_CCMRAM);

static const KAL_DATA CurSensData[OUT_COUNT][KOOF_COUNT] ={   {{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K002O20,V002O20},{K01O20,V01O20},{K10O20,V10O20},{K15O20,V15O20}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										{{K05O08,V05O08},{K10O08,V10O08},{K15O08,V15O08},{K30O08,V30O08}},
										};
/*
 *
 */
static void ADC_Init(void);
static void ADC_ReadyEvent( uint32_t event );


float GetTotalCurrent()
{
	return total_current;
}

TaskHandle_t * xGetADCTaskHandle()
{
	return  &pTaskHandle;
}



/*
 *
 */
ERROR_FLAGS_TYPE eOutGetError(OUT_NAME_TYPE eChNum )
{
	ERROR_FLAGS_TYPE error = ERROR_OFF;

   if (	eChNum < OUT_COUNT )
   {
	   switch (out[eChNum].SysReg & ERROR_MASK)
	   {
	   	   	   case OPEN_LOAD_ERROR:
	   	   		   error = ERROR_CIRCUT_BREAK;
	   	   		   break;
	   	   	   case OVERLOAD_ERROR:
	   	   		   error = ERROR_OVERLOAD;
	   	   		   break;
	   	   		default:
				   break;
	   }
   }
	return (  error );
}

/*
 *
 */
PDM_OUT_STATE_t eOutGetState ( OUT_NAME_TYPE eChNum  )
{
	PDM_OUT_STATE_t state = STATE_OUT_OFF;

	if (eChNum < OUT_COUNT)
	{

		switch ( out[eChNum].SysReg & FSM_MASK )
		{
			case  FSM_OFF_STATE:
				state =  STATE_OUT_OFF;
				break;
			case FSM_ON_PROCESS:
				state = STATE_OUT_ON_PROCESS;
				break;
			case FSM_ON_STATE:
				state =   STATE_OUT_ON;
				break;
			case FSM_ERROR_STATE:
				state =   STATE_OUT_ERROR;
				break;
			case FSM_RESTART_STATE:
				state = STATE_OUT_RESTART_PROCESS;
				break;
			default:
				break;
		}
	}
	return (state);
}

/*
 *
 */
float fOutGetMaxCurrent(OUT_NAME_TYPE eChNum)
{
	return ( (eChNum < OUT_COUNT) ? out[eChNum ].power : 0U );
}
/*
 *
 */
void vPWMFreqSet( OUT_CH_GROUPE_TYPE groupe, uint32_t Freq)
{
	if ((Freq > 0) && (Freq < 6000))
	{
       switch (groupe)
       {
           case CH5_6_9_10:
        	   vHAL_SetTimerFreq(TIMER1, Freq  );
               out[ OUT_5 ].PWM_Freg =Freq;
               out[ OUT_6 ].PWM_Freg =Freq;
               out[ OUT_9 ].PWM_Freg =Freq;
               out[ OUT_10 ].PWM_Freg =Freq;
               break;
           case CH11_12_16:
        	   vHAL_SetTimerFreq(TIMER2, Freq  );
               out[ OUT_11 ].PWM_Freg =Freq;
               out[ OUT_12 ].PWM_Freg =Freq;
               out[ OUT_16 ].PWM_Freg =Freq;
               break;
           case CH4_15:
        	   vHAL_SetTimerFreq(TIMER3, Freq  );
               out[ 3 ].PWM_Freg =Freq;
               out[ 14 ].PWM_Freg =Freq;
               break;
           case CH1_2_8_20:
        	   vHAL_SetTimerFreq(TIMER4, Freq  );;
               out[ 0 ].PWM_Freg =Freq;
               out[ 1 ].PWM_Freg =Freq;
               out[ 7 ].PWM_Freg =Freq;
               out[ 19 ].PWM_Freg =Freq;
               break;
           case CH13_14_17_18:
        	   vHAL_SetTimerFreq(TIMER8, Freq  );
               out[ 12 ].PWM_Freg =Freq;
               out[ 13 ].PWM_Freg =Freq;
               out[ 16 ].PWM_Freg =Freq;
               out[ 17 ].PWM_Freg =Freq;
               break;
           case CH7_19:
        	   vHAL_SetTimerFreq(TIMER8, Freq  );
               out[ 6 ].PWM_Freg =Freq;
               out[ 18 ].PWM_Freg =Freq;
               break;
           default:
               break;
       }
	}
   return;
}

/*
 *
 */
float fOutGetCurrent ( OUT_NAME_TYPE eChNum)
{
	return ( (eChNum < OUT_COUNT) ?  out[eChNum].current : 0U );
}

void vGetDoutStatus(uint32_t * Dout1_10Status, uint32_t * Dout11_20Status)
{
	uint32_t status1 		= 0;
	uint32_t status2 		= 0;
	uint32_t channel_state 	= 0;
	for (uint8_t i = 0; i < OUT_COUNT ;i++)
	{
		switch (out[i].SysReg & ERROR_MASK)
		{
			case OPEN_LOAD_ERROR:
				channel_state = 0x02;
				break;
			case  OVERLOAD_ERROR :
				channel_state = 0x04;
				break;
			default:
				if ( IS_FLAG_SET(i,  FSM_ON_PROCESS)  ||   IS_FLAG_SET(i,  FSM_ON_STATE ) )
				{
						channel_state = 0x01;
				}
				else
				{
						channel_state = 0x00;
				}
				break;
		  }
		if (i<10)
		{
			status1 |= channel_state<<(3*i);
		}
		else
		{
			status2 |= channel_state<<(3*(i-10));
		}

	}
	*Dout1_10Status = status1;
	*Dout11_20Status = status2;
	return;
}

 void vHWOutEnable(OUT_NAME_TYPE out_name)
{
	HAL_SetBit(out[out_name].CS_PORT, out[out_name].CS_Pin );
}

/*
 *
 */
void vHWOutDisable(OUT_NAME_TYPE out_name)
{
	HAL_ResetBit(  out[out_name].CS_PORT, out[out_name].CS_Pin);
}



/*
 *
 */
static void vHWOutSet( OUT_NAME_TYPE out_name )
{
   HAL_TIMER_EnablePWMCH(out[out_name].ptim,out[out_name].channel);
   return;
}


/*
 *
 */
void vOutSetState(OUT_NAME_TYPE out_name, uint8_t state)
{
	switch (state)
	{
		case 0:
			SET_FLAG(out_name, CONTROL_OFF_STATE);
			break;
		case 1:
			SET_FLAG(out_name, CONTROL_ON_STATE);
			break;
		default:
			break;
	}
	return;
}
/*
 *
 */
void vHWOutOFF( uint8_t ucChannel )
{
	HAL_TIMER_DisablePWMCH(out[ucChannel].ptim,out[ucChannel].channel );
	out[ucChannel].POWER_SOFT = 0;
	return;
}
/*
 * Функция конфигурации режима рестарта
 */
ERROR_CODE vHWOutResetConfig(OUT_NAME_TYPE out_name, uint8_t restart_count, uint16_t timer)
{
	ERROR_CODE res = INVALID_ARG;
	if ( out_name < OUT_COUNT )      //Проверяем корекность номера канала
	{
		RESET_FLAG(out_name,ENABLE_FLAG);;
		out[out_name].max_error_counter = restart_count;
		out[out_name].error_counter = restart_count;
		out[out_name].restart_timer = 0U;
		out[out_name].restart_config_timer = timer;
		res = ERROR_OK;
		SET_FLAG(out_name,ENABLE_FLAG);
	}
	return ( res );
}
/*
 * Функция конфигурация номинальной мощности и режима перегрузки канала, с проверкой коректности парамертов
 */
ERROR_CODE vHWOutOverloadConfig(OUT_NAME_TYPE out_name,  float power, uint16_t overload_timer, float overload_power, OFF_STATE_TYPE off_state, uint8_t filter_enable)
{
	ERROR_CODE res = INVALID_ARG;
	if (out_name < OUT_COUNT)     //Проверяем корекность номера канала
	{
		RESET_FLAG(out_name,ENABLE_FLAG);
		if ( (overload_timer <= MAX_OVERLOAD_TIMER) && ( power > 0) && (overload_power > 0) &&
					(((power <= MAX_LPOWER) &&  (overload_power<= MAX_OVERLOAD_LPOWER)) ||
					((power <= MAX_HPOWER) &&  (overload_power<= MAX_OVERLOAD_HPOWER) && ( out_name < OUT_HPOWER_COUNT) ))
			   )
			{
				out[out_name].power = power;
				out[out_name].overload_config_timer = overload_timer;
				out[out_name].overload_power = overload_power;
				out[out_name].filter_enable = filter_enable;
				if  ( off_state == RESETTEBLE_STATE_AFTER_ERROR)
				{
					SET_FLAG(out_name,RESETTEBLE_FLAG);
				}
				else
				{
					RESET_FLAG(out_name,RESETTEBLE_FLAG);
				}
				res = ERROR_OK;
			}
			SET_FLAG(out_name,ENABLE_FLAG);
	}
	return ( res );
}

void vSetRendomResetState( OUT_NAME_TYPE out_name,  uint8_t state, uint8_t cool_down)
{
	if ( out_name < OUT_COUNT )
	{
		out[out_name].RanfomOverload = (state!=0) ? 1: 0;
		out[out_name].cooldown_coof = (cool_down!=0)? cool_down : 1;
		out[out_name].cooldown_timer   = 0;
	}
}
/*
 * Функция конфигурации режима PWM
 */
ERROR_CODE vOutSetPWM(OUT_NAME_TYPE out_name, uint8_t PWM)
{
	ERROR_CODE res = INVALID_ARG;
	uint32_t pulse;
	if ((out_name <  OUT_COUNT ) && (PWM <=MAX_PWM) && (out[out_name].PWM_Freg!=0)) //Проверяем коректность агрументов
	{
		if ( out[out_name].PWM != PWM )
		{
			out[out_name].PWM = PWM;
			if (PWM < 100 )
			{
				pulse = (uint32_t )( ( TIMER_PERIOD *(float)out[out_name].PWM/ MAX_PWM ) )+1U;
			}
			else
			{
				pulse = 1001;
			}
			HAL_TIMER_SetPWMPulse( out[out_name].ptim, out[out_name].channel,  pulse );
			if (   IS_FLAG_RESET(out_name, FSM_ERROR_STATE) &&   IS_FLAG_RESET(out_name, FSM_OFF_STATE) ) //Если выход вклчюен и не находится в каком-то переходном процессе
			{
				vHWOutSet( out_name );
			}
		}
		res = ERROR_OK;
	}
	return ( res );
}

/*
 *
 */
static void vHWOutInit(OUT_NAME_TYPE out_name, TimerName_t ptim, uint8_t uiChannel, PortName_t EnablePort, uint16_t EnablePin,  PortName_t OutPort, uint16_t OutPin )
{
	volatile uint8_t j;
	if ( out_name < OUT_COUNT )
	{
		out[out_name].ptim 			   = ptim;
		out[out_name].channel 		   = uiChannel;
		out[out_name].CS_PORT 		   = EnablePort;
		out[out_name].CS_Pin		   = EnablePin;
		out[out_name].OUT_PORT		   = OutPort;
		out[out_name].OUT_Pin		   = OutPin;
		out[out_name].error_counter    = 0U;
		out[out_name].soft_start_timer = 0;
		out[out_name].current 		   = 0.0;
		out[out_name].PWM_err_counter  = 0;
		out[out_name].POWER_SOFT 	   = 0;
		out[out_name].state 		   = 0;
		out[out_name].PWM_Freg         = 0;
		out[out_name].PWM              = 100;
		out[out_name].filter_enable    = 1;
		vSetRendomResetState(out_name, 0, 1);
		//Конфигурация пина CurSens Enable
		HAL_InitGpioOut( EnablePort, EnablePin );
		vHWOutDisable( out_name);
		//Конфигурация пина PWM в режим альтернативной функции
		uint16_t AF = 0;
		switch ( ptim)
		{
			case TIMER1:
				AF = TIMER1_PWM_AF;
				break;
			case TIMER2:
				AF = TIMER2_PWM_AF;
				break;
			case TIMER3:
				AF = TIMER3_PWM_AF;
				break;
			case TIMER4:
				AF = TIMER4_PWM_AF;
				break;
			case TIMER8:
				AF = TIMER8_PWM_AF;
				break;
			case TIMER12:
				AF = TIMER12_PWM_AF;
				break;
			default:
				break;
		}
		HAL_InitGpioAF( OutPort,  OutPin,  AF ,MODE_OUT_PP );
		RESET_FLAG(out_name,CONTROL_FLAGS );
		SET_STATE_FLAG(out_name, FSM_OFF_STATE );
		if (out_name < OUT_HPOWER_COUNT)
		{
			vHWOutOverloadConfig(out_name, DEFAULT_HPOWER,DEFAULT_OVERLOAD_TIMER_HPOWER, DEFAULT_HPOWER_MAX, RESETTEBLE_STATE_AFTER_ERROR,1);
		}
		else
		{
			vHWOutOverloadConfig(out_name, DEFAULT_LPOWER,DEFAULT_OVERLOAD_TIMER_LPOWER, DEFAULT_LPOWER_MAX , RESETTEBLE_STATE_AFTER_ERROR,1);
		}
		vHWOutResetConfig(out_name,DEFAULT_RESET_COUNTER, DEFAULT_RESET_TIMER);
		vOutSetPWM(out_name, DEFAULT_PWM);
		RESET_FLAG(out_name,ENABLE_FLAG);
		RESET_FLAG(out_name, ERROR_MASK);

		for (j=0; j< KOOF_COUNT - 1 ; j++)
		{
			//Проверяем что хоты одно значение АЦП не равно нулю,что-то не словить делением на ноль.
			if ((CurSensData[out_name][j].Data != 0.0) || (CurSensData[out_name][j+1].Data != 0.0 ))
			{
			    float temp, temp1;
                vABLineKoofFinde(&temp, &temp1,
                                CurSensData[out_name][j].Data, CurSensData[out_name][j+1].Data,
                                CurSensData[out_name][j].KOOF, CurSensData[out_name][j+1].KOOF);
				out[out_name].CSC[j].data = CurSensData[out_name][j+1].Data;
				out[out_name].CSC[j].k = temp;
				out[out_name].CSC[j].b = temp1;
			}
		}
		vHWOutEnable( out_name );
	}
	return;
}



void vOutInit( void )
{
	//Инициализация всех каналов таймеров PWM
	HAL_TIMER_PWMTimersInit(TIMER4, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_TIMER_PWMTimersInit(TIMER2, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_TIMER_PWMTimersInit(TIMER1, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_TIMER_PWMTimersInit(TIMER3, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 );
	HAL_TIMER_PWMTimersInit(TIMER8, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_TIMER_PWMTimersInit(TIMER12, 1000000, 1000, TIM_CHANNEL_1 | TIM_CHANNEL_2 );
	vHWOutInit(OUT_1, TIMER4, TIM_CHANNEL_3, Cs_Dis20_1_GPIO_Port,Cs_Dis20_1_Pin, OUT1_PORT ,OUT1_PIN );
	vHWOutInit(OUT_2, TIMER4, TIM_CHANNEL_4, Cs_Dis20_2_GPIO_Port,Cs_Dis20_2_Pin, OUT2_PORT ,OUT2_PIN  );
	vHWOutInit(OUT_3, TIMER2, TIM_CHANNEL_1, Cs_Dis20_3_GPIO_Port,Cs_Dis20_3_Pin, OUT3_PORT ,OUT3_PIN );
	vHWOutInit(OUT_4, TIMER3, TIM_CHANNEL_2, Cs_Dis20_4_GPIO_Port,Cs_Dis20_4_Pin, OUT4_PORT ,OUT4_PIN );
	vHWOutInit(OUT_5, TIMER1, TIM_CHANNEL_3, Cs_Dis20_5_GPIO_Port, Cs_Dis20_5_Pin,OUT5_PORT ,OUT5_PIN );
	vHWOutInit(OUT_6, TIMER1, TIM_CHANNEL_4, Cs_Dis20_6_GPIO_Port, Cs_Dis20_6_Pin,OUT6_PORT ,OUT6_PIN );
	vHWOutInit(OUT_7, TIMER12, TIM_CHANNEL_1, Cs_Dis20_7_GPIO_Port,Cs_Dis20_7_Pin, OUT7_PORT ,OUT7_PIN );
	vHWOutInit(OUT_8, TIMER4, TIM_CHANNEL_2, Cs_Dis20_8_GPIO_Port,Cs_Dis20_8_Pin ,OUT8_PORT ,OUT8_PIN  );
	vHWOutInit(OUT_9, TIMER1, TIM_CHANNEL_1,  Cs_Dis8_9_10_GPIO_Port, Cs_Dis8_9_10_Pin,  OUT9_PORT ,OUT9_PIN  );
	vHWOutInit(OUT_10, TIMER1, TIM_CHANNEL_2, Cs_Dis8_9_10_GPIO_Port, Cs_Dis8_9_10_Pin , OUT10_PORT ,OUT10_PIN );
	vHWOutInit(OUT_11, TIMER2, TIM_CHANNEL_4, Cs_Dis8_11_12_GPIO_Port, Cs_Dis8_11_12_Pin , OUT11_PORT ,OUT11_PIN );
	vHWOutInit(OUT_12, TIMER2, TIM_CHANNEL_3, Cs_Dis8_11_12_GPIO_Port, Cs_Dis8_11_12_Pin ,OUT12_PORT ,OUT12_PIN  );
	vHWOutInit(OUT_13, TIMER8, TIM_CHANNEL_1,Cs_Dis8_13_14_GPIO_Port,Cs_Dis8_13_14_Pin , OUT13_PORT ,OUT13_PIN  );
	vHWOutInit(OUT_14, TIMER8, TIM_CHANNEL_2,Cs_Dis8_13_14_GPIO_Port, Cs_Dis8_13_14_Pin , OUT14_PORT ,OUT14_PIN );
	vHWOutInit(OUT_15, TIMER3, TIM_CHANNEL_1,Cs_Dis8_15_16_GPIO_Port, Cs_Dis8_15_16_Pin , OUT15_PORT ,OUT15_PIN  );
	vHWOutInit(OUT_16, TIMER2, TIM_CHANNEL_2, Cs_Dis8_15_16_GPIO_Port, Cs_Dis8_15_16_Pin ,OUT16_PORT ,OUT16_PIN );
	vHWOutInit(OUT_17, TIMER8, TIM_CHANNEL_3, Cs_Dis8_17_18_GPIO_Port,  Cs_Dis8_17_18_Pin , OUT17_PORT ,OUT17_PIN );
	vHWOutInit(OUT_18, TIMER8, TIM_CHANNEL_4, Cs_Dis8_17_18_GPIO_Port, Cs_Dis8_17_18_Pin , OUT18_PORT ,OUT18_PIN );
	vHWOutInit(OUT_19, TIMER12,TIM_CHANNEL_2,Cs_Dis8_19_20_GPIO_Port,Cs_Dis8_19_20_Pin , OUT19_PORT ,OUT19_PIN );
	vHWOutInit(OUT_20, TIMER4, TIM_CHANNEL_1, Cs_Dis8_19_20_GPIO_Port,Cs_Dis8_19_20_Pin,OUT20_PORT ,OUT20_PIN  );
	return;
}

/*
 *
 */
 static void vGotoRestartState( uint8_t ucChannel, float fCurr )
 {
	 if (out[ ucChannel ].error_counter == 1)
	 {
		 SET_STATE_FLAG( ucChannel, FSM_ERROR_STATE );
		 RESET_FLAG(ucChannel,CONTROL_FLAGS );
	 }
	 else
	 {
		 SET_STATE_FLAG( ucChannel, FSM_RESTART_STATE);
	 }
	 if  ( IS_FLAG_SET(ucChannel, FSM_ERROR_STATE ) &&  IS_FLAG_RESET( ucChannel, RESETTEBLE_FLAG ) )
	 {
		 RESET_FLAG(ucChannel,ENABLE_FLAG);
     }
	 SET_ERROR_FLAG( ucChannel, OVERLOAD_ERROR);
	 out[ ucChannel ].restart_timer = 0U;
	 vHWOutOFF(ucChannel);
	 if ( fCurr < ( ucChannel < OUT_HPOWER_COUNT ? MAX_OVERLOAD_HPOWER : MAX_OVERLOAD_LPOWER ) )
	 {
		 out[ucChannel].current = fCurr;
	 }

 }


static float fGetDataFromRaw( float fraw,PDM_OUTPUT_TYPE xOut)
{
	float fRes;
	for (uint8_t r = 0; r < ( KOOF_COUNT -1 ); r++)
    {
		if (( fraw < xOut.CSC[r].data ) || (r == (KOOF_COUNT -2)) )
		{
			fRes =  fraw * xOut.CSC[r].k;
			fRes += xOut.CSC[r].b ;
			fRes *= fraw/RR;
			break;
		}
	 }
	return ( fRes );
}


/*
 *
 */
static void vOutControlFSM(void)
{
	total_current = 0;
   for (uint8_t i = 0U; i < OUT_COUNT; i++ )
   {
   	if (IS_FLAG_SET(i,ENABLE_FLAG) )		/*Если канал не выключен или не в режиме конфигурации*/
   	{
   		uint16_t rawdata =  ucGetRawData( i , out[i].filter_enable);
   	    float fCurrent  =  fGetDataFromRaw( ((float) rawdata *K ) ,   out[i]);
   	    if ( ( fCurrent > out[ i ].power) && (out[i].PWM != 100) )
   	    {
   	        if  (out[i].state == RESET)
   	     	{
   	        	 fCurrent = out[i].current;
   	        	 out[i].PWM_err_counter = 0;
   	     	}
   	        else
   	        {
   	        	if  (out[i].PWM_err_counter < 2 )
   	        	 {
   	        		 	  fCurrent = out[i].current;
   	        	 }
   	       	     out[i].PWM_err_counter ++;
   	        }
   	    }

   	    switch (out[i].SysReg & FSM_MASK )
			{
				case FSM_OFF_STATE : //Состония входа - выключен
					out[i].current 	   		 = 0U;
					out[i].restart_timer   	 = 0U;
					RESET_FLAG(i,ERROR_MASK);
					out[i].cool_down_flag    = 0U;
					out[ i ].cooldown_timer  = 0U;
					break;
				case FSM_ON_PROCESS: //Состояния влючения
					out[i].restart_timer++;
					uint8_t ucCurrentPower;
					if ((out[i].soft_start_timer !=0) && (out[i].PWM_Freg != 0))
					{
					 		if  ( fCurrent  > out[i].overload_power)
					 		{
					 			vGotoRestartState(i,fCurrent);
					 			break;
					 		}
					 		if  ( out[i].restart_timer >= out[i].soft_start_timer ) //Если прошло время полонго пуска
					 		{
					 			SET_STATE_FLAG(i, FSM_ON_STATE );
					 			ucCurrentPower = MAX_POWER;
					 		}
					 		else
					 		{   //время пуска не прошоло, вычисляем текущую мощность, котору надо пдать на выход.
					 			ucCurrentPower = out[i].soft_start_power + (uint8_t) (((float)out[i].restart_timer/(float)out[i].soft_start_timer)*(MAX_POWER - out[i].soft_start_power));
					 			if (ucCurrentPower  >= MAX_POWER)
					 			{
					 				ucCurrentPower = MAX_POWER;
					 			}
					 		}
					 		vOutSetPWM(i, ucCurrentPower );
					 }
					 else
					 {
						 if ( out[i].restart_timer  >= 2 )
						 {
					 		if  ( fCurrent  > out[ i ].overload_power )
					 		{
					 			vGotoRestartState(i,fCurrent);
					 		}
					 		else
					 		{
					 			if  (fCurrent  > out[ i ].power  )
					 			{
					 					out[ i ].cooldown_timer = 0;
					 					out[ i ].cool_down_flag = 0;
					 			}
					 			if ( out[ i ].restart_timer >= out[ i ].overload_config_timer )
					 			{
					 					SET_STATE_FLAG(i, FSM_ON_STATE );
					 					out[ i ].restart_timer = 0;
					 			}
					 		}
					 	 }
					 }
					out[i].current = fCurrent;
					break;
				case FSM_ON_STATE:  // Состояние входа - включен
					out[i].current = fCurrent;
					if  (fCurrent  > out[ i ].power  )
					{
					 		if ((out[i].RanfomOverload) &&  (out[i].cool_down_flag   == 1))
					 		{
					 				out[ i ].cooldown_timer = 0;
					 				out[i].restart_timer++;
					 				if ( out[i].restart_timer  < 2 )  break;
					 				if  (( fCurrent  > out[ i ].overload_power ) ||   ( out[ i ].restart_timer >= out[ i ].overload_config_timer ))
					 				{
					 						vGotoRestartState(i,fCurrent);
					 						break;
					 				}
					 		}
					 		else
					 		{
					 			vGotoRestartState( i, fCurrent );
					 			break;
					 		}
					 }
					 else
					 {
					 		out[ i ].restart_timer = 0;
					 		if ( out[ i ].cooldown_timer <= ( out[ i ].overload_config_timer * out[ i ].cooldown_coof) )
					 		{
					 			out[i].cool_down_flag   = 0;
					 			out[ i ].cooldown_timer++;
					 		}
					 		else
					 			out[i].cool_down_flag   = 1;
					 }
					 RESET_FLAG(i,ERROR_MASK);
					 if (fCurrent  < CIRCUT_BREAK_CURRENT)
					 {
					 		SET_ERROR_FLAG(i,OPEN_LOAD_ERROR);
					 }
					 break;
				case FSM_RESTART_STATE:
					out[ i ].restart_timer++;
					 if  ( out[ i ].restart_timer >= out[ i ].restart_config_timer )
					 {
					 		SET_STATE_FLAG(i, FSM_ON_PROCESS );
					 		out[i].cool_down_flag   = 0;
					 		out[ i ].cooldown_timer  = 0;
					 		vHWOutSet( i );
					 		out[ i ].restart_timer =0;
					 		RESET_FLAG(i,ERROR_MASK);
					 		if ( out[i].error_counter !=0 )
					 		{
					 			out[i].error_counter--;
					 		}
					 }
					break;
				case FSM_ERROR_STATE:
					if (IS_FLAG_SET(i,RESETTEBLE_FLAG) && IS_FLAG_SET(i, CONTROL_OFF_STATE))
					 {
					 	RESET_FLAG(i, OVERLOAD_ERROR);
					 	SET_STATE_FLAG(i, FSM_OFF_STATE);
					 }
					break;
				default:
					break;
			}
			// Проверям управляющие сигналы. Если они изменилсь, то выключем или включаем каналы. Это нужно сделать именно тот,
			// чтобы на следующем циклые конечного автомата были актуальные данные о состонии каналов


  		 }
   	      total_current+=out[i].current;
	}
   for (uint8_t i = 0U; i < OUT_COUNT; i++ )
   {
      	if (IS_FLAG_SET(i,ENABLE_FLAG) )		/*Если канал не выключен или не в режиме конфигурации*/
      	{
      		if ( IS_FLAG_SET( i, CONTROL_OFF_STATE ) && IS_FLAG_RESET(i, FSM_OFF_STATE)   )
      		{
      				SET_STATE_FLAG(i, FSM_OFF_STATE );
      				out[i].error_counter =  out[i].max_error_counter;
      		 	 	vHWOutOFF(i);
      		 }
      		 if ( IS_FLAG_SET( i, CONTROL_ON_STATE ) &&  IS_FLAG_SET(i, FSM_OFF_STATE) )
      		 {
      		 		SET_STATE_FLAG(i, FSM_ON_PROCESS );
      		 	 	vHWOutSet( i );
      		 }
      		 RESET_FLAG(i,CONTROL_FLAGS );
      		 out[i].state = xHAL_GetOutBit(out[i].OUT_PORT, out[i].OUT_Pin);
      	}
   }
}


 /*
  *
  */

void AinNotifyTaskToStop()
{
	ulTaskNotifyValueClear(pTaskHandle, TASK_RUN_NOTIFY);
}

void AinNotifyTaskToInit()
{

	xTaskNotify(pTaskHandle, TASK_RUN_NOTIFY , eSetValueWithOverwrite);
}



 void vADCTask(void * argument)
 {
   /* USER CODE BEGIN vADCTask */
   uint32_t ulNotifiedValue;;
   ADC_Init();
   TickType_t xLastWakeTime;
   const TickType_t xPeriod = pdMS_TO_TICKS( 1 );
   xLastWakeTime = xTaskGetTickCount();
   for(;;)
   {
	   xTaskNotifyWait(0,0x00,&ulNotifiedValue,0);
	   if ( ulNotifiedValue == TASK_RUN_NOTIFY )
	   {
		        vTaskDelayUntil( &xLastWakeTime, xPeriod );							//Запускаем преобразование на 3-х АЦП
	   		    HAL_ADC_StartDMA( DMA2_CH4, (uint16_t *)getADC1Buffer(), ( ADC_FRAME_SIZE * ADC1_CHANNELS ));
	   		    HAL_ADC_StartDMA( DMA2_CH2, (uint16_t *)getADC2Buffer(), ( ADC_FRAME_SIZE * ADC2_CHANNELS ));
	   		    HAL_ADC_StartDMA( DMA2_CH0, (uint16_t *)getADC3Buffer(), ( ADC_FRAME_SIZE * ADC3_CHANNELS ));
	   		    while  (ulNotifiedValue < 3)   xTaskNotifyWaitIndexed( 1, 0, 0  ,&ulNotifiedValue,portMAX_DELAY);					//Ждем пока из обработчиков прерваний DMA прилетят уведомления об окончании преобразований
	   		    HAL_ADCDMA_Disable(ADC_1);														//Выключаем DMA и АЦП
	   		    HAL_ADCDMA_Disable(ADC_2);														//АЦП в режиме циклеческого сканирования последовательности каналов
	   		    HAL_ADCDMA_Disable(ADC_3);														//C программным запуском. Нужон его выклчюать
	   		    vDataConvertToFloat();
	   		    vOutControlFSM();
	   		    HAL_ADC_Enable(ADC_1);
	   		    HAL_ADC_Enable(ADC_2);
	   		    HAL_ADC_Enable(ADC_3);
	   		    ulTaskNotifyValueClearIndexed(NULL, 1, 0xFFFF);
	   }
	   else
	   {
		   xTaskNotifyWait(0,0x00,&ulNotifiedValue,portMAX_DELAY);
	   }
   }
   /* USER CODE END vADCTask */
 }




 static void ADC1_Ready()
 {
	 ADC_ReadyEvent(ADC1_READY);
 }

 static void ADC2_Ready()
  {
 	 ADC_ReadyEvent(ADC2_READY);
  }

 static void ADC3_Ready()
  {
 	 ADC_ReadyEvent(ADC3_READY);
  }

 static void ADC_Init(void)
 {
   uint8_t ADC1_CHANNEL[9] = { ADC_CH_4,  ADC_CH_7, ADC_CH_6, ADC_CH_5,  ADC_CH_14, ADC_CH_15, ADC_CH_8,  ADC_CH_16, ADC_CH_9};
   uint8_t ADC2_CHANNEL[7] = { ADC_CH_11, ADC_CH_0, ADC_CH_1, ADC_CH_13, ADC_CH_12, ADC_CH_3,  ADC_CH_2 };
   uint8_t ADC3_CHANNEL[9] = { ADC_CH_14, ADC_CH_9, ADC_CH_7, ADC_CH_4,  ADC_CH_15, ADC_CH_8,  ADC_CH_10, ADC_CH_6, ADC_CH_5};
   HAL_ADC_CommonConfig();
   HAL_ADC_ContiniusScanCinvertionDMA( ADC_1 ,  9 ,  ADC1_CHANNEL);
   HAL_ADC_ContiniusScanCinvertionDMA( ADC_2 ,  7 ,  ADC2_CHANNEL);
   HAL_ADC_ContiniusScanCinvertionDMA( ADC_3 ,  9 ,  ADC3_CHANNEL);
   HAL_ADC_TempEnable();
  // HAL_ADC_VrefEnable();
   HAL_ADC_Enable(ADC_1);
   HAL_ADC_Enable(ADC_2);
   HAL_ADC_Enable(ADC_3);
   HAL_DMAInitIT( DMA2_CH4 ,  PTOM, DMA_HWORD, (uint32_t)&ADC1->REGDATA, 0, DMA_CH_0,ADC_DMA_PRIOR,ADC_DMA_SUBPRIOR, &ADC1_Ready);
   HAL_DMAInitIT( DMA2_CH2 ,  PTOM, DMA_HWORD, (uint32_t)&ADC2->REGDATA, 0, DMA_CH_1,ADC_DMA_PRIOR,ADC_DMA_SUBPRIOR, &ADC2_Ready);
   HAL_DMAInitIT( DMA2_CH0 ,  PTOM, DMA_HWORD, (uint32_t)&ADC3->REGDATA, 0, DMA_CH_2,ADC_DMA_PRIOR,ADC_DMA_SUBPRIOR, &ADC3_Ready);

 }

 static void ADC_ReadyEvent( uint32_t event )
 {
	    portBASE_TYPE xHigherPriorityTaskWoken =  pdFALSE;
		xTaskNotifyIndexedFromISR( pTaskHandle,1, event , eIncrement, &xHigherPriorityTaskWoken );
	    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
 }





