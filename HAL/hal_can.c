/*
 * hal_can.c
 *
 *  Created on: 13 мая 2024 г.
 *      Author: i.dymov
 */
#include "hal_can.h"
#include "string.h"
#include "hal_irq.h"

static HAL_CAN_t CAN;

#if MCU == CH32V2
/* Time out for INAK bit */
#define INAK_TIMEOUT                 ((uint32_t)0x0000FFFF)
#define CAN_MODE_MASK                ((uint32_t) 0x00000003)
/* CAN Mailbox Transmit Request */
#define TMIDxR_TXRQ                  ((uint32_t)0x00000001)
/* CAN FCTLR Register bits */
#define FCTLR_FINIT                  ((uint32_t)0x00000001)
void   USB_HP_CAN1_TX_IRQHandler(void) __attribute__((interrupt()));  /* USB HP and CAN1 TX */
void   USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((interrupt())); /* USB LP and CAN1RX0 */
void   CAN1_RX1_IRQHandler(void) __attribute__((interrupt()));        /* CAN1 RX1 */
void   CAN1_SCE_IRQHandler(void) __attribute__((interrupt()));       /* CAN1 SCE */

#endif

void HAL_CANSetTXCallback(void (* f) ( void ))
{
	CAN.txcallback = f;
}
void HAL_CANSetRXCallback(void (* f) ( HAL_CAN_RX_FIFO_NUMBER_t))
{
	CAN.rxcallback = f;
}

void HAL_CANSetERRCallback(void (* f) ( void ))
{
	CAN.errorcallback = f;
}

void HAL_CANIntIT(  uint16_t   CANbitRate, uint8_t prior, uint8_t subprior)
{
#if MCU == APM32
	CAN_Config_T       CAN_ConfigStructure;
	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1 );
    //Конфигурация драйвера
	CAN_ConfigStructure.mode             = CAN_MODE_NORMAL;
	CAN_ConfigStructure.autoBusOffManage = ENABLE;
	CAN_ConfigStructure.autoBusOffManage = DISABLE;
	CAN_ConfigStructure.autoWakeUpMode   = DISABLE;
	CAN_ConfigStructure.nonAutoRetran    = ENABLE;
	CAN_ConfigStructure.rxFIFOLockMode   = DISABLE;
	CAN_ConfigStructure.txFIFOPriority   = DISABLE;

    /* Configure CAN timing */
      switch (CANbitRate)
      {
            case 1000:  CAN_ConfigStructure.prescaler = 2;
                   break;
            case 500:  CAN_ConfigStructure.prescaler = 4;
                   break;
            case 250:  CAN_ConfigStructure.prescaler = 8;
                   break;
            default:
            case 125:  CAN_ConfigStructure.prescaler = 16;
                  break;
            case 100:  CAN_ConfigStructure.prescaler = 20;
                 break;
            case 50:  CAN_ConfigStructure.prescaler = 120;
                 break;
            case 20:  CAN_ConfigStructure.prescaler = 300;
                 break;
            case 10:  CAN_ConfigStructure.prescaler = 600;
                 break;
      }
      CAN_ConfigStructure.syncJumpWidth =  CAN_SJW_1;
      CAN_ConfigStructure.timeSegment1 = CAN_TIME_SEGMENT1_15;
      CAN_ConfigStructure.timeSegment2 = CAN_TIME_SEGMENT2_5;
      CAN_Config(CAN1, &CAN_ConfigStructure);

     CAN_EnableInterrupt(CAN1, CAN_INT_TXME | CAN_INT_BOF | CAN_INT_F0MP | CAN_INT_F1MP);
     NVIC_EnableIRQRequest(CAN1_RX0_IRQn, prior, subprior);
     NVIC_EnableIRQRequest(CAN1_RX1_IRQn, prior, subprior);
     NVIC_EnableIRQRequest(CAN1_SCE_IRQn, prior, subprior);
#endif
#if MCU == CH32V2
     u16 CAN_Prescaler;
     RCC->APB1PRSTR |= RCC_APB1Periph_CAN1;
     RCC->APB1PRSTR &= ~RCC_APB1Periph_CAN1;
     RCC->APB1PCENR |= RCC_APB1Periph_CAN1;

     /* Configure CAN timing */
      switch (CANbitRate)
      {
          case 1000: CAN_Prescaler  = 2;
                  break;
          case 500:  CAN_Prescaler = 4;
                  break;
          case 250:  CAN_Prescaler = 8;
                  break;
         default:
          case 125:  CAN_Prescaler = 16;
                 break;
         case 100:  CAN_Prescaler = 20;
                break;
         case 50:  CAN_Prescaler = 120;
                break;
         case 20:  CAN_Prescaler = 300;
               break;
          case 10:  CAN_Prescaler = 600;
              break;
      }


      uint32_t wait_ack = 0x00000000;

      CAN1->CTLR &= (~(uint32_t)CAN_CTLR_SLEEP);
      CAN1->CTLR |= CAN_CTLR_INRQ ;

      while (((CAN1->STATR & CAN_STATR_INAK) != CAN_STATR_INAK) && (wait_ack != INAK_TIMEOUT))
      {
         wait_ack++;
      }

      if ((CAN1->STATR & CAN_STATR_INAK) == CAN_STATR_INAK)
      {
          CAN1->CTLR &= ~(uint32_t)CAN_CTLR_TTCM;
          CAN1->CTLR |= CAN_CTLR_ABOM;
          CAN1->CTLR &= ~(uint32_t)CAN_CTLR_AWUM;
          CAN1->CTLR |= CAN_CTLR_NART;
          CAN1->CTLR &= ~(uint32_t)CAN_CTLR_RFLM;
          CAN1->CTLR &= ~(uint32_t)CAN_CTLR_TXFP;

          CAN1->BTIMR = (uint32_t)((uint32_t)CAN_Mode_Normal << 30) | \
                                      ((uint32_t)CAN_SJW_4tq << 24) | \
                                      ((uint32_t)CAN_BS1_12tq << 16) | \
                                      ((uint32_t)CAN_BS2_5tq << 20) | \
                                      ((uint32_t)CAN_Prescaler - 1);
          CAN1->CTLR &= ~(uint32_t)CAN_CTLR_INRQ;
          wait_ack = 0;
          while (((CAN1->STATR & CAN_STATR_INAK) == CAN_STATR_INAK) && (wait_ack != INAK_TIMEOUT))
          {
                  wait_ack++;
          }
         if ((CAN1->STATR & CAN_STATR_INAK) != CAN_STATR_INAK)
         {
             CAN1->INTENR |= CAN_IT_TME | CAN_IT_BOF | CAN_IT_FMP0 | CAN_IT_FMP1;
             PFIC_IRQ_ENABLE_PG1(USB_LP_CAN1_RX0_IRQn,prior,subprior);
             PFIC_IRQ_ENABLE_PG1(CAN1_RX1_IRQn,prior,subprior);
             PFIC_IRQ_ENABLE_PG1(USB_HP_CAN1_TX_IRQn,prior,subprior);
             PFIC_IRQ_ENABLE_PG1(CAN1_SCE_IRQn,prior,subprior);
         }
      }
#endif
}

uint8_t HAL_CANToInitMode()
{
#if MCU == APM32
	return (CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_INIT));
#endif
#if MCU == CH32V2
	uint32_t timeout = INAK_TIMEOUT;
	CAN1->CTLR = (uint32_t)((CAN1->CTLR & (uint32_t)(~(uint32_t)CAN_CTLR_SLEEP)) | CAN_CTLR_INRQ);
	while (((CAN1->STATR & CAN_MODE_MASK) != CAN_STATR_INAK) && (timeout != 0))
	{
	      timeout--;
	}
	return ( ((CAN1->STATR & CAN_MODE_MASK) != CAN_STATR_INAK)?  CAN_ModeStatus_Failed : CAN_ModeStatus_Success);
#endif
}
uint8_t HAL_CANToOperatingMode()
{
#if MCU == APM32
	return (CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_NORMAL));
#endif
#if MCU == CH32V2
	uint32_t timeout = INAK_TIMEOUT;
	CAN1->CTLR &= (uint32_t)(~(CAN_CTLR_SLEEP|CAN_CTLR_INRQ));
	while (((CAN1->STATR & CAN_MODE_MASK) != 0) && (timeout!=0))
	{
	    timeout--;
	}
	return ( ((CAN1->STATR & CAN_MODE_MASK) != 0)?  CAN_ModeStatus_Failed : CAN_ModeStatus_Success);
#endif
}


uint8_t HAL_CANSend(CAN_TX_FRAME_TYPE *buffer)
{
#if MCU == APM32
   CAN_TxMessage_T pTXHeader;
   pTXHeader.dataLengthCode               = (uint32_t)buffer->DLC;
   if ( buffer->ident & CAN_EXT_FLAG)
   {
    	buffer->ident 		&= ~CAN_EXT_FLAG;
    	pTXHeader.extID      =  buffer->ident;
    	pTXHeader.typeID  = CAN_TYPEID_EXT;
    	pTXHeader.remoteTxReq        = (buffer->ident & FLAG_RTR) ? CAN_RTXR_REMOTE : CAN_RTXR_DATA;
    	pTXHeader.stdID 	 = 0U;
    }
    else
    {
    	pTXHeader.extID         = 0U;
    	pTXHeader.typeID        = CAN_TYPEID_STD;
    	pTXHeader.remoteTxReq   = (buffer->ident & FLAG_RTR) ? CAN_RTXR_REMOTE : CAN_RTXR_DATA;
        pTXHeader.stdID         = buffer->ident & CANID_MASK;

    }
   return CAN_TxMessage(CAN1, &pTXHeader);
#endif
#if MCU == CH32V2
   u8 Data[8];
   u8 DLC                = (uint32_t)buffer->DLC;
   u8 RTR                = (buffer->ident & FLAG_RTR) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
   u32 StdId             =  buffer->ident & CANID_MASK;
   memcpy(Data,buffer->data,buffer->DLC);
   uint8_t transmit_mailbox = 0;
   if ((CAN1->TSTATR&CAN_TSTATR_TME0) == CAN_TSTATR_TME0)
   {
       transmit_mailbox = 0;
   }
   else if ((CAN1->TSTATR&CAN_TSTATR_TME1) == CAN_TSTATR_TME1)
   {
       transmit_mailbox = 1;
   }
   else if ((CAN1->TSTATR&CAN_TSTATR_TME2) == CAN_TSTATR_TME2)
   {
       transmit_mailbox = 2;
   }
   else
   {
      return ( CAN_TxStatus_NoMailBox);
   }
   CAN1->sTxMailBox[transmit_mailbox].TXMIR &= TMIDxR_TXRQ;
   CAN1->sTxMailBox[transmit_mailbox].TXMIR |= ((StdId << 21) | RTR);
   DLC &= (uint8_t)0x0000000F;
   CAN1->sTxMailBox[transmit_mailbox].TXMDTR &= (uint32_t)0xFFFFFFF0;
   CAN1->sTxMailBox[transmit_mailbox].TXMDTR |= DLC;

   CAN1->sTxMailBox[transmit_mailbox].TXMDLR = (((uint32_t)Data[3] << 24) |
                                                        ((uint32_t)Data[2] << 16) |
                                                        ((uint32_t)Data[1] << 8) |
                                                        ((uint32_t)Data[0]));
   CAN1->sTxMailBox[transmit_mailbox].TXMDHR = (((uint32_t)Data[7] << 24) |
                                                       ((uint32_t)Data[6] << 16) |
                                                       ((uint32_t)Data[5] << 8) |
                                                       ((uint32_t)Data[4]));
   CAN1->sTxMailBox[transmit_mailbox].TXMIR |= TMIDxR_TXRQ;
   return (transmit_mailbox);
#endif
}

void HAL_CANSetFiters(uint8_t filter_index, uint32_t f1,uint32_t f2,uint32_t f3,uint32_t f4, HAL_CAN_FILTER_FIFO_t FIFO)
{
#if MCU == APM32
	CAN_FilterConfig_T  sFilterConfig;
	sFilterConfig.filterActivation = ENABLE;
	sFilterConfig.filterNumber = filter_index;
	sFilterConfig.filterFIFO  = (FIFO  == FILTER_FIFO_0) ? CAN_FILTER_FIFO_0 : CAN_FILTER_FIFO_1  ;
	sFilterConfig.filterIdHigh 		= f2 <<5U ;
    sFilterConfig.filterIdLow  		= f1 <<5U ;
	sFilterConfig.filterMaskIdHigh =  f4 <<5U ;
	sFilterConfig.filterMaskIdLow  =  f3 <<5U ;
	sFilterConfig.filterMode = CAN_FILTER_MODE_IDLIST;
	sFilterConfig.filterScale =CAN_FILTER_SCALE_16BIT;
	CAN_ConfigFilter(& sFilterConfig);
#endif
#if MCU == CH32V2
    u16 CAN_FilterIdLow       = f1 <<5;
    u16 CAN_FilterIdHigh      = f3 <<5;
    u16 CAN_FilterMaskIdLow   = f2 <<5;
    u16 CAN_FilterMaskIdHigh  = f4 <<5;
    uint16_t CAN_FilterFIFOAssignment =  (FIFO  == FILTER_FIFO_0) ?  CAN_Filter_FIFO0 :  CAN_Filter_FIFO1 ;
    uint32_t filter_number_bit_pos = 0;
    filter_number_bit_pos = ((uint32_t)1) << filter_index;
    CAN1->FCTLR |= FCTLR_FINIT;
    CAN1->FWR &= ~(uint32_t)filter_number_bit_pos;
    CAN1->FSCFGR &= ~(uint32_t)filter_number_bit_pos;
    CAN1->sFilterRegister[filter_index].FR1 =
            ((0x0000FFFF & (uint32_t)CAN_FilterMaskIdLow) << 16) |
                    (0x0000FFFF & (uint32_t)CAN_FilterIdLow);
            CAN1->sFilterRegister[filter_index].FR2 =
            ((0x0000FFFF & (uint32_t)CAN_FilterMaskIdHigh) << 16) |
                    (0x0000FFFF & (uint32_t)CAN_FilterIdHigh);
    #if defined (CH32V20x_D6)||defined (CH32V20x_D8)
        if(((*(uint32_t *) 0x40022030) & 0x0F000000) == 0)
        {
            uint32_t i;
            for(i = 0; i < 64; i++)
            {
                *(__IO uint16_t *)(0x40006000 + 512 + 4 * i) = *(__IO uint16_t *)(0x40006000 + 768 + 4 * i);
            }
        }
    #endif
    CAN1->FMCFGR |= (uint32_t)filter_number_bit_pos;
    if (CAN_FilterFIFOAssignment == CAN_Filter_FIFO0)
    {
        CAN1->FAFIFOR &= ~(uint32_t)filter_number_bit_pos;
    }
    else
    {
        CAN1->FAFIFOR |= (uint32_t)filter_number_bit_pos;
    }
    CAN1->FWR |= filter_number_bit_pos;
    CAN1->FCTLR &= ~FCTLR_FINIT;
#endif
}

HAL_CAN_ERROR_t HAL_CANGetRXMessage( HAL_CAN_RX_FIFO_NUMBER_t fifo,  CAN_FRAME_TYPE * rx_message )
{
	HAL_CAN_ERROR_t res = HAL_CAN_ERROR;
#if MCU == APM32
   CAN_RxMessage_T rx;
   CAN_RxMessage(CAN1, (fifo == HAL_RX_FIFO0) ? CAN_RX_FIFO_0 : CAN_RX_FIFO_1, &rx) ;
   //Игнорируем RTR фрайм, поскольку у него нет данных для нас.
   if ( rx.remoteTxReq != CAN_RTXR_REMOTE )
   {
   	rx_message->ident = rx.stdID;
   	rx_message->DLC = rx.dataLengthCode;
   	rx_message->filter_id = rx.filterMatchIndex;
   	memcpy(rx_message->data,rx.data, 8 );
   	res = HAL_CAN_OK;
   }
#endif
#if MCU == CH32V2
     u8 FIFONumber = (fifo == HAL_RX_FIFO0) ? CAN_FIFO0 : CAN_FIFO1;
     u32 StdId;
     u8 IDE = (uint8_t)0x04 & CAN1->sFIFOMailBox[FIFONumber].RXMIR;
      if (IDE == CAN_Id_Standard)
      {
           StdId = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[FIFONumber].RXMIR >> 21);
           u8 RTR = (uint8_t)0x02 & CAN1->sFIFOMailBox[FIFONumber].RXMIR;
           rx_message->DLC = (uint8_t)0x0F & CAN1->sFIFOMailBox[FIFONumber].RXMDTR;
           rx_message->data[0] = (uint8_t)0xFF &  CAN1->sFIFOMailBox[FIFONumber].RXMDLR;
           rx_message->data[1] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDLR >> 8);
           rx_message->data[2] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDLR >> 16);
           rx_message->data[3] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDLR >> 24);
           rx_message->data[4] = (uint8_t)0xFF &  CAN1->sFIFOMailBox[FIFONumber].RXMDHR;
           rx_message->data[5] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDHR >> 8);
           rx_message->data[6] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDHR >> 16);
           rx_message->data[7] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[FIFONumber].RXMDHR >> 24);
           if (FIFONumber == CAN_FIFO0)
                CAN1->RFIFO0 |= CAN_RFIFO0_RFOM0;
           else
               CAN1->RFIFO1 |= CAN_RFIFO1_RFOM1;
           rx_message->ident = (StdId & CAN_SFID_MASK) | ((RTR == CAN_RTR_Remote) ? FLAG_RTR : 0x00)   ;
           rx_message->filter_id = 0;
           res =HAL_CAN_OK;
      }

#endif
   return (res);

}




#if MCU == APM32
void CAN1_SCE_IRQHandler(void)
{
	CAN_ClearIntFlag(CAN1, CAN_INT_BOF );
	CAN.errorcallback();

}
void  CAN1_RX0_IRQHandler (void )
{
	CAN.rxcallback( HAL_RX_FIFO0);
	CAN_ClearIntFlag(CAN1, CAN_INT_F0MP );
}
void CAN1_RX1_IRQHandler(void)
{
	CAN.rxcallback( HAL_RX_FIFO1);
	CAN_ClearIntFlag(CAN1,  CAN_INT_F1MP );
}

void CAN1_TX_IRQHandler (void)
{

	  if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC0) == SET )
	    {
		  CAN_ClearStatusFlag (CAN1, CAN_FLAG_REQC0);
		 // xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX0_FREE, &xHigherPriorityTaskWoken );
	    }
	    if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC1) == SET )
	    {
	    	CAN_ClearStatusFlag(CAN1, CAN_FLAG_REQC1);
	    	//xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX1_FREE, &xHigherPriorityTaskWoken );
	    }
	    if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC2) == SET )
	    {
	    	CAN_ClearStatusFlag (CAN1,CAN_FLAG_REQC2);
	    	//xEventGroupSetBitsFromISR(xCANstatusEvent, CANT_TX2_FREE, &xHigherPriorityTaskWoken );
	    }
	    CAN_ClearIntFlag(CAN1, CAN_INT_TXME );
	    CAN.txcallback();
}

#endif

#if MCU == CH32V2
/*
 * Обработчик прерывания по отправки данных
 */
void   USB_HP_CAN1_TX_IRQHandler(void)
{
    //Сбрасываем флаг прерывания по всем mainbox. В данном драйвере нам нет необходимости отслеживать
    // что мы передали, а что нет.
    CAN1->TSTATR = CAN_TSTATR_RQCP0|CAN_TSTATR_RQCP1|CAN_TSTATR_RQCP2;
    CAN.txcallback();

}
/*
 * Обработчик прерывания по приему в данных в Fifo 0
 */
void   USB_LP_CAN1_RX0_IRQHandler(void)
{

	CAN.rxcallback( HAL_RX_FIFO0);
	CAN1->RFIFO0 = CAN_RFIFO0_FULL0;  //Сбрасываем флаг прерывания

}
/*
 * Обработчик прерывания по приему в данных в Fifo 1
 */
void   CAN1_RX1_IRQHandler(void)
{
	CAN.rxcallback( HAL_RX_FIFO1);
	CAN1->RFIFO1 = CAN_RFIFO1_FULL1;  //Сбрасываем флаг прерывания

}
/*
 * Обработчик прерывния общики CAN
 */
void   CAN1_SCE_IRQHandler(void)
{
    CAN1->STATR = CAN_STATR_ERRI;   //Сбрасываем флаг прерывания
    CAN.errorcallback();
}


#endif
