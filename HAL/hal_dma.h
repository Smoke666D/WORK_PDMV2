/*
 * hal_dma.h
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_DMA_H_
#define HAL_HAL_DMA_H_


#include "main.h"
#include "system_init.h"

typedef enum
{
  Stream_0,
  Stream_1,
  Stream_2,
  Stream_3,
  Stream_4,
  Stream_5,
  Stream_6,
  Stream_7,
  Stream_8,

} DMA_Stram_t;

typedef enum
{
  PTOM,
  MTOP,
} DMA_Derection_t;

typedef enum
{
  DMA_BYTE,
  DMA_HWORD,
  DMA_WORD,
} DMA_Size_t;


typedef enum
{
    DMA_CH_0,  /*!< Select DMA channel 0 */
    DMA_CH_1,  /*!< Select DMA channel 1 */
    DMA_CH_2,  /*!< Select DMA channel 2 */
    DMA_CH_3,  /*!< Select DMA channel 3 */
    DMA_CH_4,  /*!< Select DMA channel 4 */
    DMA_CH_5,  /*!< Select DMA channel 5 */
    DMA_CH_6,  /*!< Select DMA channel 6 */
    DMA_CH_7   /*!< Select DMA channel 7 */
} DMA_CHANNEL_t;


typedef struct
{
	void (* CallBack)( void );

} DMA_CFG_t;

void HAL_ADC_StartDMA( DMA_Stram_t chanel, uint16_t * data, uint16_t size);
void DMA2_STR4_IRQHandler( void );
void DMA2_STR2_IRQHandler( void );
void DMA2_STR0_IRQHandler( void );
void HAL_DMA2InitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, DMA_CHANNEL_t channel, void (*f)(void));

#endif /* HAL_HAL_DMA_H_ */
