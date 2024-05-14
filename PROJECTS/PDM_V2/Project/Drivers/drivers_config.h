/*
 * drivers_config.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_DRIVERS_CONFIG_H_
#define DRIVERS_DRIVERS_CONFIG_H_


#include "board_define.h"



#define USB_PULLUP_Pin GPIO_PIN_6
#define USB_PULLUP_GPIO_Port GPIOG
#define USB_VBAT_DET_Pin GPIO_PIN_7
#define USB_VBAT_DET_GPIO_Port GPIOG
#define Din1_Pin GPIO_PIN_0
#define Din1_Port PORT_E
#define Din2_Pin GPIO_PIN_1
#define Din2_Port PORT_E
#define Din3_Pin GPIO_PIN_2
#define Din3_Port PORT_E
#define Din4_Pin GPIO_PIN_3
#define Din4_Port PORT_E
#define Din5_Pin GPIO_PIN_4
#define Din5_Port PORT_E
#define Din6_Pin GPIO_PIN_5
#define Din6_Port PORT_E
#define Din7_Pin GPIO_PIN_6
#define Din7_Port PORT_E
#define Din8_Pin GPIO_PIN_9
#define Din8_Port PORT_B
#define Din9_Pin GPIO_PIN_8
#define Din9_Port PORT_B
#define Din10_Pin GPIO_PIN_13
#define Din10_Port PORT_F
#define Din11_Pin GPIO_PIN_14
#define Din11_Port PORT_F
#define Din12_Pin GPIO_PIN_15
#define Din12_Port PORT_F
#define CS20_1_Pin GPIO_PIN_4
#define CS20_1_GPIO_Port PORT_F
#define CS20_2_Pin GPIO_PIN_3
#define CS20_2_GPIO_Port PORT_F
#define CS20_3_Pin GPIO_PIN_9
#define CS20_3_GPIO_Port PORT_F
#define CS20_4_Pin GPIO_PIN_1
#define CS20_4_GPIO_Port PORT_C
#define CS20_5_Pin GPIO_PIN_0
#define CS20_5_GPIO_Port PORT_A
#define CS20_6_Pin GPIO_PIN_1
#define CS20_6_GPIO_Port PORT_A
#define CS20_7_Pin GPIO_PIN_4
#define CS20_7_GPIO_Port PORT_A
#define CS20_8_Pin GPIO_PIN_7
#define CS20_8_GPIO_Port PORT_A
#define CS8_9_Pin GPIO_PIN_3
#define CS8_9_GPIO_Port PORT_C
#define CS8_10_Pin GPIO_PIN_2
#define CS8_10_GPIO_Port PORT_C
#define CS8_11_Pin GPIO_PIN_3
#define CS8_11_GPIO_Port PORT_A
#define CS8_12_Pin GPIO_PIN_2
#define CS8_12_GPIO_Port PORT_A
#define CS8_13_Pin GPIO_PIN_6
#define CS8_13_GPIO_Port PORT_F
#define CS8_14_Pin GPIO_PIN_5
#define CS8_14_GPIO_Port PORT_F
#define CS8_15_Pin GPIO_PIN_10
#define CS8_15_GPIO_Port PORT_F
#define CS8_16_Pin GPIO_PIN_0
#define CS8_16_GPIO_Port PORT_C
#define CS8_18_Pin GPIO_PIN_7
#define CS8_18_GPIO_Port PORT_F
#define CS8_17_Pin GPIO_PIN_8
#define CS8_17_GPIO_Port PORT_F
#define CS8_20_Pin GPIO_PIN_5
#define CS8_20_GPIO_Port PORT_A
#define CS8_19_Pin GPIO_PIN_6
#define CS8_19_GPIO_Port PORT_A

#define ADC1_4_Pin GPIO_PIN_4
#define ADC1_4_GPIO_Port PORT_C
#define ADC1_5_Pin GPIO_PIN_5
#define ADC1_5_GPIO_Port PORT_C
#define ADC1_8_Pin GPIO_PIN_0
#define ADC1_8_GPIO_Port PORT_B
#define ADC1_9_Pin GPIO_PIN_1
#define ADC1_9_GPIO_Port PORT_B
#define Din10_Pin GPIO_PIN_13
#define Din10_GPIO_Port GPIOF
#define Din11_Pin GPIO_PIN_14
#define Din11_GPIO_Port GPIOF
#define Din12_Pin GPIO_PIN_15
#define Din12_GPIO_Port GPIOF


#define Cs_Dis20_1_Pin GPIO_PIN_3
#define Cs_Dis20_1_GPIO_Port PORT_G
#define Cs_Dis20_2_Pin GPIO_PIN_2
#define Cs_Dis20_2_GPIO_Port PORT_G
#define Cs_Dis20_3_Pin GPIO_PIN_10
#define Cs_Dis20_3_GPIO_Port PORT_G
#define Cs_Dis20_4_Pin GPIO_PIN_11
#define Cs_Dis20_4_GPIO_Port PORT_G
#define Cs_Dis20_5_Pin GPIO_PIN_1
#define Cs_Dis20_5_GPIO_Port PORT_G
#define Cs_Dis20_6_Pin GPIO_PIN_7
#define Cs_Dis20_6_GPIO_Port PORT_E
#define Cs_Dis20_7_Pin GPIO_PIN_9
#define Cs_Dis20_7_GPIO_Port PORT_D
#define Cs_Dis20_8_Pin GPIO_PIN_11
#define Cs_Dis20_8_GPIO_Port PORT_D
#define Cs_Dis8_9_10_Pin GPIO_PIN_8
#define Cs_Dis8_9_10_GPIO_Port PORT_E
#define Cs_Dis8_11_12_Pin GPIO_PIN_8
#define Cs_Dis8_11_12_GPIO_Port PORT_D
#define Cs_Dis8_13_14_Pin GPIO_PIN_4
#define Cs_Dis8_13_14_GPIO_Port PORT_G
#define Cs_Dis8_15_16_Pin GPIO_PIN_9
#define Cs_Dis8_15_16_GPIO_Port PORT_G
#define Cs_Dis8_17_18_Pin GPIO_PIN_5
#define Cs_Dis8_17_18_GPIO_Port PORT_G
#define Cs_Dis8_19_20_Pin GPIO_PIN_10
#define Cs_Dis8_19_20_GPIO_Port PORT_D

#define InCH20_1_Pin GPIO_PIN_14
#define InCH20_1_GPIO_Port PORT_D
#define InCH20_2_Pin GPIO_PIN_15
#define InCH20_2_GPIO_Port PORT_D
#define InCH20_3_Pin GPIO_PIN_15
#define InCH20_3_GPIO_Port PORT_A
#define InCH20_4_Pin GPIO_PIN_5
#define InCH20_4_GPIO_Port PORT_B
#define InCH20_5_Pin GPIO_PIN_13
#define InCH20_5_GPIO_Port PORT_E
#define InCH20_6_Pin GPIO_PIN_14
#define InCH20_6_GPIO_Port PORT_E
#define InCH20_7_Pin GPIO_PIN_14
#define InCH20_7_GPIO_Port PORT_B
#define InCH20_8_Pin GPIO_PIN_13
#define InCH20_8_GPIO_Port PORT_D
#define InCH8_9_Pin GPIO_PIN_9
#define InCH8_9_GPIO_Port PORT_E
#define InCH8_10_Pin GPIO_PIN_11
#define InCH8_10_GPIO_Port PORT_E
#define InCH8_11_Pin GPIO_PIN_11
#define InCH8_11_GPIO_Port PORT_B
#define InCH8_12_Pin GPIO_PIN_10
#define InCH8_12_GPIO_Port PORT_B
#define InCH8_13_Pin GPIO_PIN_6
#define InCH8_13_GPIO_Port PORT_C
#define InCH8_14_Pin GPIO_PIN_7
#define InCH8_14_GPIO_Port PORT_C
#define InCH8_15_Pin GPIO_PIN_4
#define InCH8_15_GPIO_Port PORT_B
#define InCH8_16_Pin GPIO_PIN_3
#define InCH8_16_GPIO_Port PORT_B
#define InCH8_17_Pin GPIO_PIN_8
#define InCH8_17_GPIO_Port PORT_C
#define InCH8_18_Pin GPIO_PIN_9
#define InCH8_18_GPIO_Port PORT_C
#define InCH8_19_Pin GPIO_PIN_15
#define InCH8_19_GPIO_Port PORT_B
#define InCH8_20_Pin GPIO_PIN_12
#define InCH8_20_GPIO_Port PORT_D

#define OUT1_PIN  InCH20_1_Pin
#define OUT1_PORT InCH20_1_GPIO_Port
#define OUT2_PIN  InCH20_2_Pin
#define OUT2_PORT InCH20_2_GPIO_Port
#define OUT3_PIN  InCH20_3_Pin
#define OUT3_PORT InCH20_3_GPIO_Port
#define OUT4_PIN  InCH20_4_Pin
#define OUT4_PORT InCH20_4_GPIO_Port
#define OUT5_PIN  InCH20_5_Pin
#define OUT5_PORT InCH20_5_GPIO_Port
#define OUT6_PIN  InCH20_6_Pin
#define OUT6_PORT InCH20_6_GPIO_Port
#define OUT7_PIN  InCH20_7_Pin
#define OUT7_PORT InCH20_7_GPIO_Port
#define OUT8_PIN  InCH20_8_Pin
#define OUT8_PORT InCH20_8_GPIO_Port
#define OUT9_PIN  InCH8_9_Pin
#define OUT9_PORT InCH8_9_GPIO_Port
#define OUT10_PIN  InCH8_10_Pin
#define OUT10_PORT InCH8_10_GPIO_Port
#define OUT11_PIN  InCH8_11_Pin
#define OUT11_PORT InCH8_11_GPIO_Port
#define OUT12_PIN  InCH8_12_Pin
#define OUT12_PORT InCH8_12_GPIO_Port
#define OUT13_PIN  InCH8_13_Pin
#define OUT13_PORT InCH8_13_GPIO_Port
#define OUT14_PIN  InCH8_14_Pin
#define OUT14_PORT InCH8_14_GPIO_Port
#define OUT15_PIN  InCH8_15_Pin
#define OUT15_PORT InCH8_15_GPIO_Port
#define OUT16_PIN  InCH8_16_Pin
#define OUT16_PORT InCH8_16_GPIO_Port
#define OUT17_PIN  InCH8_17_Pin
#define OUT17_PORT InCH8_17_GPIO_Port
#define OUT18_PIN  InCH8_18_Pin
#define OUT18_PORT InCH8_18_GPIO_Port
#define OUT19_PIN  InCH8_19_Pin
#define OUT19_PORT InCH8_19_GPIO_Port
#define OUT20_PIN  InCH8_20_Pin
#define OUT20_PORT InCH8_20_GPIO_Port

#define 			IPS_OUTS



#define EEPROM_I2C  I2C_2
/***********************************/
//Конфигурация DIN
/**********************************/
#define DEF_H_FRONT 10U
#define DEF_L_FRONT 10U
#define RPM_CHANNEL_COUNT     2
#define DIN_CHANNEL_COUNT     12
#define DIN_COUNT             12
#define DOUT_COUNT            0


#define DIN_CHANNEL 			11U
#define VELOCITY_COUNT      2U     // Количество каналов скорости
#define TEMPERATURE_COUNT   1U     /* Количество каналов температуры */
#define ANGLE_COUNT         2U
/*
 * Конфигурация AIN
 */

    #define BAT_INDEX           3U
    #define TEMP_INDEX           4U
    #define ADC1_CHANNELS       9U
    #define ADC2_CHANNELS       7U
    #define ADC3_CHANNELS       9U
    #define AIN_NUMBER          3U        //Количесвто аналоговых входов
    #define OUT_COUNT           20U
    #define AIN_COUNT  			3U


typedef enum {
	ERROR_OFF,
	ERROR_OVER_LIMIT,
	ERROR_OVERLOAD,
	ERROR_CIRCUT_BREAK,
	ERROR_VDD_SHORTCUT,
	ERROR_VSS_SHORTCUT
} ERROR_FLAGS_TYPE;

typedef enum {
  STATE_OUT_OFF,
  STATE_OUT_ON_PROCESS,
  STATE_OUT_ON,
  STATE_OUT_ERROR,
  STATE_OUT_RESTART_PROCESS
} PDM_OUT_STATE_t;



#if MCU == CH32
    #include "ch32v20x_tim.h"
	#define TIMER_Type TIM_TypeDef
#endif
#if MCU == APM32
	#include "apm32f4xx_tmr.h"
    #include "apm32f4xx_rcm.h"
	#define TIMER_Type TMR_T
#endif

void vHWLilBaseTimerInit(TIMER_Type * tim );
uint16_t GetBaseTick();
uint16_t GetDelay( uint16_t start_delay);

#endif /* DRIVERS_DRIVERS_CONFIG_H_ */
