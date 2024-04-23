/*
 * hal_i2c.h
 *
 *  Created on: 20 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_I2C_H_
#define HAL_HAL_I2C_H_

#include "main.h"
#include "system_init.h"


#define PLATFORM  APM32

#if PLATFORM == APM32
#include "apm32f4xx_i2c.h"
#define I2C_t I2C_T *
#endif

typedef enum
{
I2C_1 = 0,
I2C_2 = 1,
} I2C_NAME_t;

void InitI2CDMA( I2C_NAME_t i2c);
void I2C1_EV_IRQHandler( void );
void I2C1_ER_IRQHandler ( void );
void I2C2_EV_IRQHandler( void );
void I2C2_ER_IRQHandler ( void );
void DMA1_STR3_IRQHandler( void );
void DMA1_STR7_IRQHandler( void );

#endif /* HAL_HAL_I2C_H_ */
