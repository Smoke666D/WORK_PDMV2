/*
 * hal_spi.h
 *
 *  Created on: 14 мая 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "hal_config.h"

#if MCU == APM32
	#include "apm32f4xx_spi.h"
	#define HAL_SPI_t SPI_T*

#endif

#if MCU == CH32
	#include "ch32v20x_spi.h"
	#include "ch32v20x_dma.h"

	#define HAL_SPI_t SPI_TypeDef *
#endif

void HAL_SPI_InitDMA(HAL_SPI_t spi );

#endif /* HAL_HAL_SPI_H_ */
