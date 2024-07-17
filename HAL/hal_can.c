/*
 * hal_can.c
 *
 *  Created on: 13 мая 2024 г.
 *      Author: i.dymov
 */
#include "hal_can.h"
#include "string.h"

static HAL_CAN_t CAN;

#if MCU == CH32V2
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
	 uint16_t            prescaler;
	//Включаем тактирование шины и перезапускаем переферию
	RCM->APB1CLKEN |= RCM_APB1_PERIPH_CAN1;
	RCM->AHB1RST   |= RCM_APB1_PERIPH_CAN1;
	RCM->AHB1RST &= (uint32_t)~RCM_APB1_PERIPH_CAN1;
    /* Configure CAN timing */
    switch (CANbitRate)
    {
        case 1000:  prescaler = 2;
                   break;
        case 500:  prescaler = 4;
                   break;
        case 250: prescaler = 8;
                   break;
            default:
        case 125:  prescaler = 16;
                  break;
        case 100:  prescaler = 20;
                 break;
        case 50:  prescaler = 120;
                 break;
        case 20:  prescaler = 300;
                 break;
        case 10:  prescaler = 600;
                 break;
      }
      uint8_t  initStatus = ERROR;
      uint32_t wait_ack = 0x00000000;

      /* Exit from sleep mode */
      CAN1->MCTRL_B.SLEEPREQ = BIT_RESET;
      /* Request initialisation */
      CAN1->MCTRL_B.INITREQ = BIT_SET;

      /* Wait the acknowledge */
      while(((CAN1->MSTS_B.INITFLG) != BIT_SET) && (wait_ack != 0x0000FFFF))
      {
            wait_ack++;
      }
      /* Check acknowledge */
      if(((CAN1->MSTS_B.INITFLG) == BIT_SET))
      {

            CAN1->MCTRL_B.ALBOFFM = BIT_SET;  //Enable autoBusOffManage
             CAN1->MCTRL_B.AWUPCFG = BIT_RESET; //Disable autoWakeUpMode
             CAN1->MCTRL_B.ARTXMD = BIT_RESET;  //Disable nonAutoRetran
             CAN1->MCTRL_B.RXFLOCK = BIT_RESET; //Disable .rxFIFOLockMode
            CAN1->MCTRL_B.TXFPCFG = BIT_SET; //Enable txFIFOPriority

            /* Set the bit timing register */
            CAN1->BITTIM &= (uint32_t)0x3fffffff;
            CAN1->BITTIM |= (uint32_t)CAN_MODE_NORMAL << 30;
            CAN1->BITTIM_B.RSYNJW  = CAN_SJW_1;
            CAN1->BITTIM_B.TIMSEG1 = CAN_TIME_SEGMENT1_15;
            CAN1->BITTIM_B.TIMSEG2 =  CAN_TIME_SEGMENT2_5;
            CAN1->BITTIM_B.BRPSC   = prescaler - 1;

            /* Request leave initialisation */
            CAN1->MCTRL_B.INITREQ = BIT_RESET;

            wait_ack = 0;
            /* Wait the acknowledge */
            while(((CAN1->MSTS_B.INITFLG) != BIT_RESET) && (wait_ack != 0x0000FFFF))
            {
                wait_ack++;
            }
            /* Check acknowledge */
            if(((CAN1->MSTS_B.INITFLG) != BIT_RESET))
            {
                initStatus = ERROR;
            }
            else
            {
                initStatus = SUCCESS;
            }
        }


     if (initStatus!= ERROR)
     {
    	 CAN1->INTEN |=  ( CAN_INT_TXME |  CAN_INT_F0MP | CAN_INT_F1MP  );
    	 NVIC_EnableIRQRequest(CAN1_RX0_IRQn, prior, subprior);
    	 NVIC_EnableIRQRequest(CAN1_RX1_IRQn, prior, subprior);
    	 NVIC_EnableIRQRequest(CAN1_SCE_IRQn, prior, subprior);
    	 NVIC_EnableIRQRequest(CAN1_TX_IRQn, prior, subprior);
     }
#endif
#if MCU == CH32V2
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
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = subprior;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prior;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
     NVIC_Init(&NVIC_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
     NVIC_Init(&NVIC_InitStructure);
#endif
}

uint8_t HAL_CANToInitMode()
{
#if MCU == APM32
	uint8_t states = 0;
	uint32_t time_out = 0x0000FFFF;
	/* Request initialisation */
	CAN1->MCTRL_B.SLEEPREQ = BIT_RESET;
	CAN1->MCTRL_B.INITREQ = BIT_SET;

	/* Wait the acknowledge */
	while((CAN1->MSTS_B.INITFLG != BIT_SET && CAN1->MSTS_B.SLEEPFLG != BIT_RESET) && (time_out != 0))
	{
	     time_out --;
	}
	if((CAN1->MSTS_B.INITFLG == BIT_SET && CAN1->MSTS_B.SLEEPFLG == BIT_RESET))
	{
	            states = 1;
	}
	return (states);
#endif
#if MCU == CH32V2
	return (CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization));
#endif
}
uint8_t HAL_CANToOperatingMode()
{
#if MCU == APM32
	uint8_t states = 0;
    uint32_t time_out = 0x0000FFFF;
	CAN1->MCTRL_B.SLEEPREQ = BIT_RESET;
	CAN1->MCTRL_B.INITREQ = BIT_RESET;
	time_out = 0x0000FFFF;
	while((CAN1->MSTS_B.INITFLG != BIT_RESET || CAN1->MSTS_B.SLEEPFLG != BIT_RESET) && (time_out != 0))
	{
	   time_out --;
	}
	if((CAN1->MSTS_B.INITFLG == BIT_RESET || CAN1->MSTS_B.SLEEPFLG == BIT_RESET))
	{
	   states = 1;
	}
	return (states);
#endif
#if MCU == CH32V2
	return ( CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Normal));
#endif
}


uint8_t HAL_CANSend(CAN_TX_FRAME_TYPE *buffer)
{
#if MCU == APM32
       uint8_t transmit_milbox = 0;

       /* Select one empty transmit mailbox */
       if((CAN1->TXSTS & 0x04000000) == 0x04000000)
       {
           transmit_milbox = 0;
       }
       else if((CAN1->TXSTS & 0x08000000) == 0x08000000)
       {
           transmit_milbox = 1;
       }
       else if((CAN1->TXSTS & 0x10000000) == 0x10000000)
       {
           transmit_milbox = 2;
       } else
       {
           /* No mailbox is empty*/
           return 3;
       }
       u32 remoteTxReq        = (buffer->ident & FLAG_RTR) ? CAN_RTXR_REMOTE : CAN_RTXR_DATA;
       /* Set up the Id */
       CAN1->sTxMailBox[transmit_milbox].TXMID &= 0x00000001;
       if ( buffer->ident & CAN_EXT_FLAG)
       {
    	   CAN1->sTxMailBox[transmit_milbox].TXMID |= (buffer->ident << 3) | CAN_TYPEID_EXT | (remoteTxReq);
       }
       else
       {
    	   CAN1->sTxMailBox[transmit_milbox].TXMID |= ((buffer->ident & CANID_MASK) << 21) | (remoteTxReq);
       }
       /* Set up the TXDLEN */
       CAN1->sTxMailBox[transmit_milbox].TXDLEN &= (uint32_t)0xFFFFFFF0;
       CAN1->sTxMailBox[transmit_milbox].TXDLEN |=  (uint32_t)(buffer->DLC & 0x0F);
       /* Set up the data field */
       CAN1->sTxMailBox[transmit_milbox].TXMDL = ((uint32_t)buffer->data[3] << 24) | ((uint32_t)buffer->data[2] << 16)
               | ((uint32_t)buffer->data[1] << 8) | ((uint32_t)buffer->data[0]);
       CAN1->sTxMailBox[transmit_milbox].TXMDH = ((uint32_t)buffer->data[7] << 24) | ((uint32_t)buffer->data[6] << 16)
               | ((uint32_t)buffer->data[5] << 8) | ((uint32_t)buffer->data[4]);
       /* Request transmission */
       CAN1->sTxMailBox[transmit_milbox].TXMID |= 0x00000001;

       return transmit_milbox;
#endif
#if MCU == CH32V2
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

	CAN_FILTER_FIFO_T filterFIFO  = (FIFO  == FILTER_FIFO_0) ? CAN_FILTER_FIFO_0 : CAN_FILTER_FIFO_1  ;
	u16 filterIdHigh 		= f3 <<5U ;
    u16 filterIdLow  		= f1 <<5U ;
	u16 filterMaskIdHigh 	=  f4 <<5U ;
	u16 filterMaskIdLow  	=  f2 <<5U ;

	CAN1->FCTRL_B.FINITEN = BIT_SET;
	CAN1->FACT &= ~(1 << filter_index);

	/* Filter Scale AN_FILTER_SCALE_16BIT */
	CAN1->FSCFG &= ~(1 << filter_index);

	CAN1->sFilterRegister[filter_index].FBANK1 =
	            ((0x0000FFFF & filterMaskIdLow) << 16) |
	            (0x0000FFFF & filterIdLow);

	CAN1->sFilterRegister[filter_index].FBANK2 =
	            ((0x0000FFFF & filterMaskIdHigh) << 16) |
	            (0x0000FFFF & filterIdHigh);

	/* Filter Mode */
	CAN1->FMCFG |= (1 << filter_index);   //CAN_FILTER_MODE_IDLIST


	/* Filter FIFO assignment */
	if(filterFIFO == CAN_FILTER_FIFO_0)
	{
	    CAN1->FFASS &= ~(1 << filter_index);
	}
	if(filterFIFO == CAN_FILTER_FIFO_1)
	{
	    CAN1->FFASS |= (1 << filter_index);
	}

	/* Filter activation */
	 CAN1->FACT |= (1 << filter_index);
	 CAN1->FCTRL_B.FINITEN = BIT_RESET;
#endif
#if MCU == CH32V2
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
     CAN_RX_FIFO_T FIFONumber =( fifo == HAL_RX_FIFO0) ? CAN_RX_FIFO_0 : CAN_RX_FIFO_1;
    uint32_t            typeID = ((uint8_t)0x04 & (CAN1->sRxMailBox[FIFONumber].RXMID));
    if(typeID == CAN_TYPEID_STD)
    {
    	rx_message->ident = (CAN1->sRxMailBox[FIFONumber].RXMID >> 21) & 0x000007FF;
    	uint32_t            remoteTxReq = CAN1->sRxMailBox[FIFONumber].RXMID_B.RFTXREQ;

    	//Игнорируем RTR фрайм, поскольку у него нет данных для нас.
    	if ( remoteTxReq != CAN_RTXR_REMOTE )
    	{
    		rx_message->DLC = CAN1->sRxMailBox[FIFONumber].RXDLEN_B.DLCODE;
    		rx_message->filter_id = CAN1->sRxMailBox[FIFONumber].RXDLEN_B.FMIDX;
    		/* Get the data field */
    		rx_message->data[0] = CAN1->sRxMailBox[FIFONumber].RXMDL_B.DATABYTE0;
    		rx_message->data[1] = CAN1->sRxMailBox[FIFONumber].RXMDL_B.DATABYTE1;
    		rx_message->data[2] = CAN1->sRxMailBox[FIFONumber].RXMDL_B.DATABYTE2;
    		rx_message->data[3] = CAN1->sRxMailBox[FIFONumber].RXMDL_B.DATABYTE3;
    		rx_message->data[4] = CAN1->sRxMailBox[FIFONumber].RXMDH_B.DATABYTE4;
    		rx_message->data[5] = CAN1->sRxMailBox[FIFONumber].RXMDH_B.DATABYTE5;
    		rx_message->data[6] = CAN1->sRxMailBox[FIFONumber].RXMDH_B.DATABYTE6;
    		rx_message->data[7] = CAN1->sRxMailBox[FIFONumber].RXMDH_B.DATABYTE7;
    		res = HAL_CAN_OK;
    	}
    }
    if(FIFONumber == CAN_RX_FIFO_0)
    {
        CAN1->RXF0_B.RFOM0 = BIT_SET;
    }
    else
    {
        CAN1->RXF1_B.RFOM1 = BIT_SET;
    }
#endif
#if MCU == CH32V2
   CanRxMsg rcvMsg;
       CAN_Receive(CAN1, (fifo == HAL_RX_FIFO0) ? CAN_FIFO0 : CAN_FIFO1,  &rcvMsg);


       //if ( rcvMsg.RTR != CAN_RTR_Remote )
       //{
         	rx_message->ident = (rcvMsg.StdId & CAN_SFID_MASK) | ((rcvMsg.RTR == CAN_RTR_Remote) ? FLAG_RTR : 0x00)   ;
         	rx_message->DLC   = rcvMsg.DLC;;
         	rx_message->filter_id = 0;
         	memcpy(rx_message->data,rcvMsg.Data, 8 );
         	res = HAL_CAN_OK;
      // }
#endif
   return (res);

}




#if MCU == APM32
void CAN1_SCE_IRQHandler(void)
{
	//CAN_ClearIntFlag(CAN1, CAN_INT_BOF );
	if (CAN1->TXSTS & (CAN_TSR_ALST0 | CAN_TSR_TERR0 ) )
	{
		CAN1->TXSTS |= CAN_TSR_ABRQ0;
	}
	if (CAN1->TXSTS & (CAN_TSR_ALST1 | CAN_TSR_TERR1 ))
	{
		CAN1->TXSTS |= CAN_TSR_ABRQ1;
	}
	if (CAN1->TXSTS & (CAN_TSR_ALST2 |  CAN_TSR_TERR2))
	{
		CAN1->TXSTS |= CAN_TSR_ABRQ2;
	}
	CAN.errorcallback();

}
void  CAN1_RX0_IRQHandler (void )
{
	CAN.rxcallback( HAL_RX_FIFO0);
	//CAN_ClearIntFlag(CAN1, CAN_INT_F0MP );
}
void CAN1_RX1_IRQHandler(void)
{
	CAN.rxcallback( HAL_RX_FIFO1);
	//CAN_ClearIntFlag(CAN1,  CAN_INT_F1MP );
}

void CAN1_TX_IRQHandler (void)
{
    //if ()
	  if (( CAN1->TXSTS & (CAN_FLAG_REQC0 & 0x000FFFFF)) != RESET )
	    {
		  //CAN_ClearStatusFlag (CAN1, CAN_FLAG_REQC0);
		  CAN1->TXSTS = CAN_FLAG_REQC0   & 0x000FFFFF;
	    }
	  if (( CAN1->TXSTS & (CAN_FLAG_REQC1 & 0x000FFFFF)) != RESET )
	   // if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC1) == SET )
	    {
	    	//CAN_ClearStatusFlag(CAN1, CAN_FLAG_REQC1);
	    	CAN1->TXSTS = CAN_FLAG_REQC1   & 0x000FFFFF;
	    }
	  if (( CAN1->TXSTS & (CAN_FLAG_REQC2 & 0x000FFFFF)) != RESET )
	   // if ( CAN_ReadStatusFlag(CAN1,CAN_FLAG_REQC2) == SET )
	    {
	    	//CAN_ClearStatusFlag (CAN1,CAN_FLAG_REQC2);
	    	CAN1->TXSTS = CAN_FLAG_REQC2   & 0x000FFFFF;
	    }
	    CAN1->TXSTS_B.REQCFLG0 = BIT_SET;
	    CAN1->TXSTS_B.REQCFLG1 = BIT_SET;
	    CAN1->TXSTS_B.REQCFLG2 = BIT_SET;
	    CAN.txcallback();
}

#endif

#if MCU == CH32V2
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
