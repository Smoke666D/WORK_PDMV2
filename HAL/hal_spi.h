/*
 * hal_spi.h
 *
 *  Created on: 14 мая 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "hal_config.h"


typedef enum
{
	SPI_8bit,
	SPI_16bit
} SPI_DATA_Size_t;

typedef enum
{
	SPI_HARD_NSS,
	SPI_SOFT_NSS
} SPI_NSS_t;

#if MCU == APM32
	#include "apm32f4xx_spi.h"
	#define HAL_SPI_t SPI_T*

#endif

#if MCU == CH32V2
	#include "ch32v20x_spi.h"
	#include "ch32v20x_dma.h"

	#define HAL_SPI_t SPI_TypeDef *
#endif

void HAL_SPI_InitDMA(HAL_SPI_t spi , SPI_DATA_Size_t data_size , SPI_NSS_t nss);
void HAL_SPI_RXOveleyClear(HAL_SPI_t spi );
uint8_t HAL_SPI_GetBusy(HAL_SPI_t spi );

#endif /* HAL_HAL_SPI_H_ */
