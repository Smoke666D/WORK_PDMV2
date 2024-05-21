/*
 * hal_can.h
 *
 *  Created on: 13 мая 2024 г.
 *      Author: i.dymov
 */

#ifndef HAL_HAL_CAN_H_
#define HAL_HAL_CAN_H_

#include "hal_config.h"
#include "main.h"

#define CAN_EXT_FLAG   0x80000000
/* CAN masks for identifiers */
#define CANID_MASK                              0x07FF  /*!< CAN standard ID mask */
#define FLAG_RTR                                0x8000  /*!< RTR flag, part of identifier */

#if MCU == APM32
#include "apm32f4xx_can.h"
#include "apm32f4xx_rcm.h"
#endif
#if MCU == CH32V2
#include "ch32v20x_can.h"
#include "ch32v20x_rcc.h"
#endif

typedef enum
{
	FILTER_FIFO_0,
	FILTER_FIFO_1,
} HAL_CAN_FILTER_FIFO_t;

typedef enum
{
	HAL_RX_FIFO0,
	HAL_RX_FIFO1
} HAL_CAN_RX_FIFO_NUMBER_t;

/* Transmit message object */
typedef struct {
    uint32_t ident;
    uint8_t DLC;
    uint8_t data[8];

} CAN_TX_FRAME_TYPE;




typedef struct {
	uint32_t ident;
	uint8_t  DLC;
    uint8_t  data[8];
    uint16_t filter_id;
} CAN_FRAME_TYPE;

typedef enum
{
  HAL_CAN_OK,
  HAL_CAN_ERROR,
} HAL_CAN_ERROR_t;


typedef struct
{
  void (* rxcallback) ( HAL_CAN_RX_FIFO_NUMBER_t);
  void (* txcallback) ( void );
  void (* errorcallback)(void );
} HAL_CAN_t;

#if MCU == APM32
	void CAN1_SCE_IRQHandler(void);
	void CAN1_RX0_IRQHandler (void );
	void CAN1_RX1_IRQHandler(void);
	void CAN1_TX_IRQHandler (void);
#endif

#if MCU == CH32

	void   USB_HP_CAN1_TX_IRQHandler(void);
	void   USB_LP_CAN1_RX0_IRQHandler(void);
	void   CAN1_RX1_IRQHandler(void);
	void   CAN1_SCE_IRQHandler(void);
#endif


void HAL_CANSetTXCallback(void (* f) ( void ));
void HAL_CANSetRXCallback(void (* f) ( HAL_CAN_RX_FIFO_NUMBER_t));
void HAL_CANSetERRCallback(void (* f) ( void ));
void HAL_CANIntIT(  uint16_t   CANbitRate, uint8_t prior, uint8_t subprior);
uint8_t HAL_CANToInitMode();
uint8_t HAL_CANToOperatingMode();
uint8_t HAL_CANSend(CAN_TX_FRAME_TYPE *buffer);
HAL_CAN_ERROR_t HAL_CANGetRXMessage( HAL_CAN_RX_FIFO_NUMBER_t fifo,  CAN_FRAME_TYPE * rx_message );
void HAL_CANSetFiters(uint8_t filter_index, uint32_t f1,uint32_t f2,uint32_t f3,uint32_t f4, HAL_CAN_FILTER_FIFO_t FIFO);

#endif /* HAL_HAL_CAN_H_ */
