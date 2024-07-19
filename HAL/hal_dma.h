/*
 * hal_dma.h
 *
 *  Created on: 25 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_DMA_H_
#define HAL_HAL_DMA_H_


#include "main.h"
#include "hal_config.h"
#include "system_init.h"


#if MCU == APM32
#include "apm32f4xx_dma.h"

#define DMA_Stram_t DMA_Stream_T*
#define DMA2_CH0  	DMA2_Stream0
#define DMA2_CH1  	DMA2_Stream1
#define DMA2_CH2  	DMA2_Stream2
#define DMA2_CH3  	DMA2_Stream3
#define DMA2_CH4  	DMA2_Stream4
#define DMA2_CH5  	DMA2_Stream5
#define DMA2_CH6  	DMA2_Stream6
#define DMA2_CH7  	DMA2_Stream7
#define DMA2_CH8  	DMA2_Stream8
#endif
#if MCU == CH32V2
#define DMA_Stram_t DMA_Channel_TypeDef *

#define DMA1_CH1  	DMA1_Channel1
#define DMA1_CH2  	DMA1_Channel2
#define DMA1_CH3  	DMA1_Channel3
#define DMA1_CH4  	DMA1_Channel4
#define DMA1_CH5  	DMA1_Channel5
#define DMA1_CH6  	DMA1_Channel6
#define DMA1_CH7  	DMA1_Channel7
#define DMA1_CH8  	DMA1_Channel8
#endif


#if MCU== APM32
typedef enum
{
  PTOM = DMA_DIR_PERIPHERALTOMEMORY,
  MTOP = DMA_DIR_MEMORYTOPERIPHERAL,
} DMA_Derection_t;

#endif

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


void HAL_DMA_Enable(DMA_Stram_t stream  );
void HAL_DMA_Disable(DMA_Stram_t stream  );
void HAL_DMA_SetCounter( DMA_Stram_t stream, uint32_t counter );
void HAL_DMA_TCEnable( DMA_Stram_t chanel);
void HAL_DMA_SerSource( DMA_Stram_t chanel, uint16_t * data, uint16_t size );
#if MCU == APM32
void DMA2_STR4_IRQHandler( void );
void DMA2_STR2_IRQHandler( void );
void DMA2_STR0_IRQHandler( void );
#endif
void HAL_DMAInitIT( DMA_Stram_t stream , DMA_Derection_t direction, DMA_Size_t dma_size, uint32_t paddr, uint32_t memadr, DMA_CHANNEL_t channel, uint8_t prior, uint8_t subprior, void (*f)(void));

#endif /* HAL_HAL_DMA_H_ */
