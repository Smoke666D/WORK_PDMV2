/*
 * hal_spi.h
 *
 *  Created on: 14 灞戦偑瑜� 2024 璋�.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "hal_config.h"


#if MCU == CH32V2
#include "ch32v20x_spi.h"

typedef enum
{
    SPI_8bit = SPI_DataSize_8b,
    SPI_16bit = SPI_DataSize_16b
} SPI_DATA_Size_t;

#else
typedef enum
{
    SPI_8bit,
    SPI_16bit
} SPI_DATA_Size_t;
#endif



typedef enum
{
    SPI_HARD_NSS,
    SPI_SOFT_NSS
} SPI_NSS_t;

typedef enum
{
   HAL_SPI1 =0,
   HAL_SPI2 = 1
} HAL_SPI_t;


void HAL_SPI_InitDMA(HAL_SPI_t spi , SPI_DATA_Size_t data_size );
void HAL_SPI_RXOveleyClear(HAL_SPI_t spi );
uint8_t HAL_SPI_GetBusy(HAL_SPI_t spi );

#endif /* HAL_HAL_SPI_H_ */
