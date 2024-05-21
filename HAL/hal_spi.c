/*
 * HAL_SPI.c
 *
 *  Created on: 14 мая 2024 г.
 *      Author: i.dymov
 */


#include "hal_spi.h"



void HAL_SPI_InitDMA(HAL_SPI_t spi , SPI_DATA_Size_t data_size , SPI_NSS_t nss)
{
#if MCU == CH32V2
	SPI_InitTypeDef  SPI_InitStructure = {0};
	if ( spi == SPI1 )  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	if ( spi == SPI2 )  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	SPI_I2S_DeInit(spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = ( data_size == SPI_8bit) ? SPI_DataSize_8b : SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = (nss == SPI_SOFT_NSS) ? SPI_NSS_Soft : SPI_NSS_Hard ;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB ;
    SPI_InitStructure.SPI_CRCPolynomial = 0;
    SPI_Init(spi, &SPI_InitStructure);
    SPI_I2S_DMACmd(spi, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_Cmd(spi, ENABLE);
#endif

}

void HAL_SPI_RXOveleyClear(HAL_SPI_t spi )
{
#if MCU == CH32V2
	SPI_I2S_ClearFlag(spi,SPI_I2S_FLAG_OVR);


#endif

}

uint8_t HAL_SPI_GetBusy(HAL_SPI_t spi )
{
#if MCU == CH32V2
	return ((SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_BSY) == SET) ? HAL_SET : HAL_RESET);
#endif
}
