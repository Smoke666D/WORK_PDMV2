/*
 * hal_gpio.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_


#include "main.h"

#define TIMER1_PWM_AF GPIO_AF_TMR1
#define TIMER2_PWM_AF GPIO_AF_TMR2
#define TIMER3_PWM_AF GPIO_AF_TMR3
#define TIMER4_PWM_AF GPIO_AF_TMR4
#define TIMER8_PWM_AF GPIO_AF_TMR8
#define TIMER12_PWM_AF GPIO_AF_TMR12
#define TIMER10_AF GPIO_AF_TMR10
#define TIMER9_AF GPIO_AF_TMR9

typedef enum
{
   PORT_A = 0,
   PORT_B = 1,
   PORT_C = 2,
   PORT_D = 3,
   PORT_E = 4,
   PORT_F = 5,
   PORT_G = 6,
   PORT_H = 7,
} PortName_t;

typedef enum
{
  PORT_INIT = 1,
  PORT_NOTINIT = 0,
} PortInitState_t;


typedef enum
{
  GPIO_IN,
  GPIO_OUT_NOPP,
  GPIO_OUT_PUP,
  GPIO_OUT_PDOWN,
} PortType_t;

typedef enum
{
  HAL_BIT_RESET = 0,
  HAL_BIT_SET   = 1,
} BitState_t;

void HAL_InitGpioLib();
void HAL_InitGpioAF(PortName_t PORT, uint16_t Pin, uint16_t AF , GPIO_MODE_T mode );
void HAL_InitGpioInPUP(PortName_t PORT, uint16_t Pin);
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  );
void vHAL_SetBit(  PortName_t  port, uint16_t pin );
void vHAL_ResetBit(  PortName_t  port, uint16_t pin );
void HAL_InitGpioAIN(PortName_t PORT, uint16_t Pin );
BitState_t xHAL_GetBit( PortName_t  port, uint16_t pin);
BitState_t xHAL_GetOutBit( PortName_t  port, uint16_t pin);

#endif /* HAL_HAL_GPIO_H_ */
