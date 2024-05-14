/*
 * hal_can.c
 *
 *  Created on: 13 мая 2024 г.
 *      Author: i.dymov
 */
#include "hal_can.h"
#include "string.h"

static HAL_CAN_t CAN;

#if MCU == CH32
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

void HAL_CANInt(  uint16_t   CANbitRate)
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
     NVIC_EnableIRQRequest(CAN1_RX0_IRQn, 5, 0);
     NVIC_EnableIRQRequest(CAN1_RX1_IRQn, 5, 0);
     NVIC_EnableIRQRequest(CAN1_SCE_IRQn, 5, 0);
#endif
#if MCU == CH32
     NVIC_InitTypeDef      NVIC_InitStructure = {0};
     CAN_InitTypeDef       CAN_InitSturcture = {0};
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
     CAN_InitSturcture.CAN_Mode = CAN_Mode_Normal;
     CAN_InitSturcture.CAN_SJW  = CAN_SJW_4tq;
     CAN_InitSturcture.CAN_BS1  = CAN_BS1_12tq;
     CAN_InitSturcture.CAN_BS2  = CAN_BS2_5tq;
     CAN_InitSturcture.CAN_TTCM = DISABLE;
     CAN_InitSturcture.CAN_ABOM = ENABLE;
     CAN_InitSturcture.CAN_AWUM = DISABLE;
     CAN_InitSturcture.CAN_NART = ENABLE;
     CAN_InitSturcture.CAN_RFLM = DISABLE;
     CAN_InitSturcture.CAN_TXFP = DISABLE;
     /* Configure CAN timing */
      switch (CANbitRate)
      {
          case 1000: CAN_InitSturcture.CAN_Prescaler  = 2;
                  break;
          case 500:  CAN_InitSturcture.CAN_Prescaler = 4;
                  break;
          case 250:  CAN_InitSturcture.CAN_Prescaler = 8;
                  break;
         default:
          case 125:  CAN_InitSturcture.CAN_Prescaler = 16;
                 break;
         case 100:  CAN_InitSturcture.CAN_Prescaler = 20;
                break;
         case 50:  CAN_InitSturcture.CAN_Prescaler = 120;
                break;
         case 20:  CAN_InitSturcture.CAN_Prescaler = 300;
               break;
        case 10:  CAN_InitSturcture.CAN_Prescaler = 600;
              break;
      }
     CAN_Init(CAN1, &CAN_InitSturcture);
     CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
     CAN_ClearITPendingBit(CAN1, CAN_IT_TME );
     CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
     CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
     CAN_ITConfig(CAN1, CAN_IT_TME | CAN_IT_BOF | CAN_IT_FMP0 | CAN_IT_FMP1 ,ENABLE);
     NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
     NVIC_Init(&NVIC_InitStructure);
#endif
}

void HAL_CANToInitMode()
{
#if MCU == APM32
	CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_INIT);
#endif
#if MCU == CH32
	CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization);
#endif
}
void HAL_CANToOperatingMode()
{
#if MCU == APM32
		CAN_OperatingMode(CAN1, CAN_OPERATING_MODE_NORMAL);
#endif
#if MCU == CH32
	CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Normal);
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
#if MCU == CH32
    CanTxMsg pTXHeader          =  {0,0,CAN_Id_Standard,0,0,{0}};
    pTXHeader.DLC                = (uint32_t)buffer->DLC;
    pTXHeader.RTR                = (buffer->ident & FLAG_RTR) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    pTXHeader.StdId              =  buffer->ident & CANID_MASK;
    for (int i=0;i<buffer->DLC;i++)
    {
        pTXHeader.Data[i] = buffer->data[i];
    }
    return ( CAN_Transmit(CAN1, &pTXHeader));

#endif
}

void HAL_CANSetFiters(uint8_t filter_index, uint32_t f1,uint32_t f2,uint32_t f3,uint32_t f4, HAL_CAN_FILTER_FIFO_t FIFO)
{
#if MCU == APM32
	CAN_FilterConfig_T  sFilterConfig;
	sFilterConfig.filterActivation = ENABLE;
	sFilterConfig.filterNumber = filter_index;
	sFilterConfig.filterFIFO  = (FIFO  == FILTER_FIFO_0) ? CAN_FILTER_FIFO_0 : CAN_FILTER_FIFO_1  ;
	sFilterConfig.filterIdHigh 		= f3 <<5U ;
    sFilterConfig.filterIdLow  		= f1 <<5U ;
	sFilterConfig.filterMaskIdHigh =  f4 <<5U ;
	sFilterConfig.filterMaskIdLow  =  f2 <<5U ;
	sFilterConfig.filterMode = CAN_FILTER_MODE_IDLIST;
	sFilterConfig.filterScale =CAN_FILTER_SCALE_16BIT;
	CAN_ConfigFilter(& sFilterConfig);
#endif
#if MCU == CH32
	CAN_FilterInitTypeDef  sFilterConfig;
	sFilterConfig.CAN_FilterMode  = CAN_FilterMode_IdList;
	sFilterConfig.CAN_FilterScale = CAN_FilterScale_16bit;
	sFilterConfig.CAN_FilterIdLow =   f1 <<5;
	sFilterConfig.CAN_FilterIdHigh =  f3 <<5;
	sFilterConfig.CAN_FilterMaskIdLow  = f2 <<5;
	sFilterConfig.CAN_FilterMaskIdHigh = f4 <<5;
	sFilterConfig.CAN_FilterFIFOAssignment =  (FIFO  == FILTER_FIFO_0) ?  CAN_Filter_FIFO0 :  CAN_Filter_FIFO1 ;
	sFilterConfig.CAN_FilterNumber = filter_index;
	sFilterConfig.CAN_FilterActivation =ENABLE;
	CAN_FilterInit(&sFilterConfig);
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
#if MCU == CH32
    CO_CANrxMsg_t rcvMsg;
    CAN_GetRxMessage(CAN1, (fifo == HAL_RX_FIFO0) ? CAN_FIFO0 : CAN_FIFO1,  &rcvMsg);
    if ( rx.remoteTxReq != CAN_RTXR_REMOTE )
    {
      	rx_message->ident = rcvMsg.ident;
      	rx_message->DLC   = rcvMsg.dlc;;
      	rx_message->filter_id = 0;
      	memcpy(rx_message->data,rcvMsg.data, 8 );
      	res = HAL_CAN_OK;
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

#if MCU == CH32
void   USB_HP_CAN1_TX_IRQHandler(void)
{


    if ( CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP0) == SET )
    {
        CAN_ClearFlag (CAN1, CAN_FLAG_RQCP0);
    }
    if ( CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP1) == SET )
    {
        CAN_ClearFlag (CAN1, CAN_FLAG_RQCP1);

    }
    if ( CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP2) == SET )
    {
        CAN_ClearFlag (CAN1, CAN_FLAG_RQCP2);

    }
    CAN.txcallback();
    CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
}
void   USB_LP_CAN1_RX0_IRQHandler(void)
{

	CAN.rxcallback( HAL_RX_FIFO0);
	CAN_ClearITPendingBit( CAN1, CAN_IT_FF0);

}
void   CAN1_RX1_IRQHandler(void)
{
	CAN.rxcallback( HAL_RX_FIFO1);
	CAN_ClearITPendingBit( CAN1, CAN_IT_FF1);

}
void   CAN1_SCE_IRQHandler(void)
{
	CAN_ClearITPendingBit( CAN1, CAN_IT_BOF);
    CAN.errorcallback();
}


#endif
