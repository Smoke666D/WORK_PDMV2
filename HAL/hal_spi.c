/*
 * HAL_SPI.c
 *
 *  Created on: 14 мая 2024 г.
 *      Author: i.dymov
 */


#include "hal_spi.h"



void HAL_SPI_InitDMA(HAL_SPI_t spi )
{
#if MCU == CH32
	SPI_InitTypeDef  SPI_InitStructure = {0};
	if ( spi == SPI1 )  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	if ( spi == SPI2 )  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	SPI_I2S_DeInit(spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB ;
    SPI_InitStructure.SPI_CRCPolynomial = 0;
    SPI_Init(spi, &SPI_InitStructure);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
#endif

}


