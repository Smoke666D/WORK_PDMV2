/*
 * adc_task.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_ADC_TASK_H_
#define DRIVERS_ADC_TASK_H_


#include <hal_timers.h>
#include "main.h"
#include "hw_lib_adc.h"
#include "drivers_config.h"
#include "hw_lib_din.h"
#include "hal_gpio.h"

#define TIMER_PERIOD  1000u








#define CS_ENABLE	   		SET
#define CS_DISABLE	   		RESET

#define DEFAULT_HPOWER      30.0  // Номинальный ток по умолчания для мощных каналов
#define MAX_HPOWER			30.0  // Максимальный номинальый ток для мощных каналов
//#define MAX_HOVERLOAD_POWER 60.0
#define DEFAULT_LPOWER      8.0  // Номинальый ток маломощных каналов
#define MAX_LPOWER			15.0  //Максимальный номинальый ток для маломощных каналов
//#define MAX_LOVERLOAD_POWER 20.0
#define DEFAULT_OVERLOAD_TIMER_HPOWER   1000U //Время плавного пуска для мощных каналов
#define MAX_OVERLOAD_TIMER             32000U //Максимальное время плавного пуска для мощных каналов
#define DEFAULT_OVERLOAD_TIMER_LPOWER   0U //Время плавного пуска для маломощнвх каналов
#define DEFAULT_HPOWER_MAX              60.0 // Ток перегрузки при старте для мощных каналов
#define MAX_OVERLOAD_HPOWER             120.0 // Максиальный пусковой ток мощных каналов
#define DEFAULT_LPOWER_MAX              15.0 // Ток перегрузки при старте для маломощных каналов
#define MAX_OVERLOAD_LPOWER             40.0 // Максиальный пусковой ток маломощных каналов
#define DEFAULT_PWM				100U
#define MAX_PWM					100U
#define MAX_POWER				100U
#define DEFAULT_RESET_TIMER		1000U
#define DEFAULT_RESET_COUNTER	1U
#define MAX_RESET_TIMER         32767U

#define KOOF_COUNT 4


#define OUT_BUSY		   0x08
#define ADC_DATA_READY     0x10
#define ADC1_CHANNELS      9U
#define ADC2_CHANNELS      7U
#define ADC3_CHANNELS      9U
#define ADC_FRAME_SIZE     3U
#define R1  10000.0
#define R2  3000.0
#define R3  1500.0

#define K   ( 3.3 / 0xFFF )



#define RA1  60400.0
#define RA2  3000.0
#define RA3  10000.0
#define AINCOOF1  ( ( RA1 + RA3 ) /RA3) * K
#define AINCOOF2  ( ( RA3 + RA3 ) /RA3) * K
#define OUT_HPOWER_COUNT    8U

#define INDIOD  0.45
#define K   ( 3.3 / 0xFFF )

#define RR  300.0
#define K15O20   16450U
#define V15O20   (float)(15.0/K15O20*RR)
#define K10O20   16450U
#define V10O20	 (float)(10.0/K10O20*RR)
#define K01O20    16650U
#define V01O20	 (float)(1.0/K01O20*RR)
#define K002O20  16800U
#define V002O20  (float)(0.20/K002O20*RR)

#define K30O08  15520U
#define V30O08	(float)(30.0/K30O08*RR)
#define K15O08  16910U
#define V15O08	(float)(15.0/K15O08*RR)
#define K10O08  16110U
#define V10O08	(float)(10.0/K10O08*RR)
#define K05O08  15520U
#define V05O08	(float)(5.0/K05O08*RR)
#define ERROR_CURRENT  (uint16_t)(4000U)


#define CIRCUT_BREAK_CURRENT  0.17

#define COOF  ( ( R1 + R2 ) /R2) * K
#define COOF1  ( ( R1 + R3 ) /R3) * K

#define STATE_OUT_CONFIG		  0x20

typedef enum
{
	ADC_IDLE_STATE,
	ADC_INIT_STATE,
	ADC_RUN1_STATE,
	ADC_WHAIT_CONVERSION_STATE,
	ADC_RUN2_STATE,
} ADC_Task_State_t;


typedef struct __packed
{
   PortName_t CS_PORT;
   uint16_t   CS_Pin;
   PortName_t OUT_PORT;
   uint16_t   OUT_Pin;
   TimerName_t  ptim;
   uint8_t  channel;
   float power;
   uint8_t state;
   uint8_t cooldown_coof;
   uint32_t cooldown_timer;
   uint8_t cool_down_flag;
   uint8_t RanfomOverload;
   float overload_power;
   float current;
   uint8_t filter_enable;
   uint8_t error_counter;
   uint8_t max_error_counter;
   uint8_t PWM;
   uint8_t PWM_err_counter;
   uint8_t POWER_SOFT;
   uint16_t PWM_Freg;
   uint8_t soft_start_power;
   uint16_t soft_start_timer;
   uint16_t overload_config_timer;
   uint16_t restart_timer;
   uint16_t restart_config_timer;
   uint32_t SysReg;
   LIN_COOF CSC[KOOF_COUNT -1 ];

} PDM_OUTPUT_TYPE;


//Результаты выполнения функций
typedef enum {
  ERROR_OK = 0,
  INVALID_ARG = 1,
} ERROR_CODE;

typedef enum {
    CH5_6_9_10    = 0,
    CH11_12_16    = 1,
    CH4_15          = 2,
    CH1_2_8_20      = 3,
    CH13_14_17_18 = 4,
    CH7_19          = 5,
} OUT_CH_GROUPE_TYPE;


typedef enum {
  OFF_STATE_AFTER_ERROR = 0 ,
  RESETTEBLE_STATE_AFTER_ERROR = 1,
} OFF_STATE_TYPE;

#define ENABLE_FLAG  	  0x00000001
#define RESETTEBLE_FLAG   0x00000002
#define CONTROL_ON_STATE  0x00000004
#define CONTROL_OFF_STATE 0x00000008
#define FSM_OFF_STATE     0x00000010
#define FSM_ON_PROCESS    0x00000020
#define FSM_ON_STATE      0x00000040
#define FSM_ERROR_STATE   0x00000080
#define FSM_RESTART_STATE 0x00000100
#define OVERLOAD_ERROR    0x00000200
#define OPEN_LOAD_ERROR   0x00000400
#define CONTROL_FLAGS ( CONTROL_OFF_STATE | CONTROL_ON_STATE )

#define FSM_MASK  		  0x000001F0
#define ERROR_MASK		  0x00000600

#define  RESET_FLAG(i, flag) (out[i].SysReg &= ~flag )
#define  SET_FLAG(i, flag) (out[i].SysReg |= flag )
#define  SET_STATE_FLAG(i, flag) (out[i].SysReg = (out[i].SysReg & ~FSM_MASK) | flag )
#define  SET_ERROR_FLAG(i, flag) (out[i].SysReg = (out[i].SysReg & ~ERROR_MASK) | flag )
#define  IS_FLAG_SET(i, flag)  ( ( (out[i].SysReg & flag ) == flag ) )
#define  IS_FLAG_RESET(i, flag) ( ( (out[i].SysReg & flag ) != flag ) )

EventGroupHandle_t * pADCEventGet();
void AinNotifyTaskToStop();
void AinNotifyTaskToInit();
void vADCTask(void * argument);
TaskHandle_t * xGetADCTaskHandle();
ERROR_FLAGS_TYPE eOutGetError(OUT_NAME_TYPE eChNum );
PDM_OUT_STATE_t eOutGetState ( OUT_NAME_TYPE eChNum  );
float fOutGetMaxCurrent(OUT_NAME_TYPE eChNum);
void vOutInit( void );
float fOutGetCurrent ( OUT_NAME_TYPE eChNum);
ERROR_CODE vOutSetPWM(OUT_NAME_TYPE out_name, uint8_t PWM);
void vOutSetState(OUT_NAME_TYPE out_name, uint8_t state);
void vHWOutOFF( uint8_t ucChannel );
void vGetDoutStatus(uint32_t * Dout1_10Status, uint32_t * Dout11_20Status);
void vHWOutEnable(OUT_NAME_TYPE out_name);
float GetTotalCurrent();
void vHWOutDisable(OUT_NAME_TYPE out_name);
ERROR_CODE vHWOutResetConfig(OUT_NAME_TYPE out_name, uint8_t restart_count, uint16_t timer);

ERROR_CODE vHWOutOverloadConfig(OUT_NAME_TYPE out_name,  float power, uint16_t overload_timer, float overload_power, OFF_STATE_TYPE off_state, uint8_t filter_enable);
void vPWMFreqSet( OUT_CH_GROUPE_TYPE groupe, uint32_t Freq);
void vSetRendomResetState( uint8_t out_name,  uint8_t state, uint8_t cool_down);
#endif /* DRIVERS_ADC_TASK_H_ */
