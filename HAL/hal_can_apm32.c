/*
 * hal_can.c
 *
 *  Created on: 13 мая 2024 г.
 *      Author: i.dymov
 */
#include "hal_can.h"


#if MCU == APM32


#include "apm32f4xx_can.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx.h"


#define CAN_TSR_RQCP0_Pos      (0U)
#define CAN_TSR_RQCP0_Msk      (0x1UL << CAN_TSR_RQCP0_Pos)                     /*!< 0x00000001 */
#define CAN_TSR_RQCP0          CAN_TSR_RQCP0_Msk                               /*!<Request Completed Mailbox0 */
#define CAN_TSR_TXOK0_Pos      (1U)
#define CAN_TSR_TXOK0_Msk      (0x1UL << CAN_TSR_TXOK0_Pos)                     /*!< 0x00000002 */
#define CAN_TSR_TXOK0          CAN_TSR_TXOK0_Msk                               /*!<Transmission OK of Mailbox0 */
#define CAN_TSR_ALST0_Pos      (2U)
#define CAN_TSR_ALST0_Msk      (0x1UL << CAN_TSR_ALST0_Pos)                     /*!< 0x00000004 */
#define CAN_TSR_ALST0          CAN_TSR_ALST0_Msk                               /*!<Arbitration Lost for Mailbox0 */
#define CAN_TSR_TERR0_Pos      (3U)
#define CAN_TSR_TERR0_Msk      (0x1UL << CAN_TSR_TERR0_Pos)                     /*!< 0x00000008 */
#define CAN_TSR_TERR0          CAN_TSR_TERR0_Msk                               /*!<Transmission Error of Mailbox0 */
#define CAN_TSR_ABRQ0_Pos      (7U)
#define CAN_TSR_ABRQ0_Msk      (0x1UL << CAN_TSR_ABRQ0_Pos)                     /*!< 0x00000080 */
#define CAN_TSR_ABRQ0          CAN_TSR_ABRQ0_Msk                               /*!<Abort Request for Mailbox0 */
#define CAN_TSR_RQCP1_Pos      (8U)
#define CAN_TSR_RQCP1_Msk      (0x1UL << CAN_TSR_RQCP1_Pos)                     /*!< 0x00000100 */
#define CAN_TSR_RQCP1          CAN_TSR_RQCP1_Msk                               /*!<Request Completed Mailbox1 */
#define CAN_TSR_TXOK1_Pos      (9U)
#define CAN_TSR_TXOK1_Msk      (0x1UL << CAN_TSR_TXOK1_Pos)                     /*!< 0x00000200 */
#define CAN_TSR_TXOK1          CAN_TSR_TXOK1_Msk                               /*!<Transmission OK of Mailbox1 */
#define CAN_TSR_ALST1_Pos      (10U)
#define CAN_TSR_ALST1_Msk      (0x1UL << CAN_TSR_ALST1_Pos)                     /*!< 0x00000400 */
#define CAN_TSR_ALST1          CAN_TSR_ALST1_Msk                               /*!<Arbitration Lost for Mailbox1 */
#define CAN_TSR_TERR1_Pos      (11U)
#define CAN_TSR_TERR1_Msk      (0x1UL << CAN_TSR_TERR1_Pos)                     /*!< 0x00000800 */
#define CAN_TSR_TERR1          CAN_TSR_TERR1_Msk                               /*!<Transmission Error of Mailbox1 */
#define CAN_TSR_ABRQ1_Pos      (15U)
#define CAN_TSR_ABRQ1_Msk      (0x1UL << CAN_TSR_ABRQ1_Pos)                     /*!< 0x00008000 */
#define CAN_TSR_ABRQ1          CAN_TSR_ABRQ1_Msk                               /*!<Abort Request for Mailbox 1 */
#define CAN_TSR_RQCP2_Pos      (16U)
#define CAN_TSR_RQCP2_Msk      (0x1UL << CAN_TSR_RQCP2_Pos)                     /*!< 0x00010000 */
#define CAN_TSR_RQCP2          CAN_TSR_RQCP2_Msk                               /*!<Request Completed Mailbox2 */
#define CAN_TSR_TXOK2_Pos      (17U)
#define CAN_TSR_TXOK2_Msk      (0x1UL << CAN_TSR_TXOK2_Pos)                     /*!< 0x00020000 */
#define CAN_TSR_TXOK2          CAN_TSR_TXOK2_Msk                               /*!<Transmission OK of Mailbox 2 */
#define CAN_TSR_ALST2_Pos      (18U)
#define CAN_TSR_ALST2_Msk      (0x1UL << CAN_TSR_ALST2_Pos)                     /*!< 0x00040000 */
#define CAN_TSR_ALST2          CAN_TSR_ALST2_Msk                               /*!<Arbitration Lost for mailbox 2 */
#define CAN_TSR_TERR2_Pos      (19U)
#define CAN_TSR_TERR2_Msk      (0x1UL << CAN_TSR_TERR2_Pos)                     /*!< 0x00080000 */
#define CAN_TSR_TERR2          CAN_TSR_TERR2_Msk                               /*!<Transmission Error of Mailbox 2 */
#define CAN_TSR_ABRQ2_Pos      (23U)
#define CAN_TSR_ABRQ2_Msk      (0x1UL << CAN_TSR_ABRQ2_Pos)                     /*!< 0x00800000 */
#define CAN_TSR_ABRQ2          CAN_TSR_ABRQ2_Msk                               /*!<Abort Request for Mailbox 2 */
#define CAN_TSR_CODE_Pos       (24U)
#define CAN_TSR_CODE_Msk       (0x3UL << CAN_TSR_CODE_Pos)                      /*!< 0x03000000 */
#define CAN_TSR_CODE           CAN_TSR_CODE_Msk                                /*!<Mailbox Code */

#define CAN_TSR_TME_Pos        (26U)
#define CAN_TSR_TME_Msk        (0x7UL << CAN_TSR_TME_Pos)                       /*!< 0x1C000000 */
#define CAN_TSR_TME            CAN_TSR_TME_Msk                                 /*!<TME[2:0] bits */
#define CAN_TSR_TME0_Pos       (26U)
#define CAN_TSR_TME0_Msk       (0x1UL << CAN_TSR_TME0_Pos)                      /*!< 0x04000000 */
#define CAN_TSR_TME0           CAN_TSR_TME0_Msk                                /*!<Transmit Mailbox 0 Empty */
#define CAN_TSR_TME1_Pos       (27U)
#define CAN_TSR_TME1_Msk       (0x1UL << CAN_TSR_TME1_Pos)                      /*!< 0x08000000 */
#define CAN_TSR_TME1           CAN_TSR_TME1_Msk                                /*!<Transmit Mailbox 1 Empty */
#define CAN_TSR_TME2_Pos       (28U)
#define CAN_TSR_TME2_Msk       (0x1UL << CAN_TSR_TME2_Pos)                      /*!< 0x10000000 */
#define CAN_TSR_TME2           CAN_TSR_TME2_Msk                                /*!<Transmit Mailbox 2 Empty */

#define CAN_TSR_LOW_Pos        (29U)
#define CAN_TSR_LOW_Msk        (0x7UL << CAN_TSR_LOW_Pos)                       /*!< 0xE0000000 */
#define CAN_TSR_LOW            CAN_TSR_LOW_Msk                                 /*!<LOW[2:0] bits */
#define CAN_TSR_LOW0_Pos       (29U)
#define CAN_TSR_LOW0_Msk       (0x1UL << CAN_TSR_LOW0_Pos)                      /*!< 0x20000000 */
#define CAN_TSR_LOW0           CAN_TSR_LOW0_Msk                                /*!<Lowest Priority Flag for Mailbox 0 */
#define CAN_TSR_LOW1_Pos       (30U)
#define CAN_TSR_LOW1_Msk       (0x1UL << CAN_TSR_LOW1_Pos)                      /*!< 0x40000000 */
#define CAN_TSR_LOW1           CAN_TSR_LOW1_Msk                                /*!<Lowest Priority Flag for Mailbox 1 */
#define CAN_TSR_LOW2_Pos       (31U)
#define CAN_TSR_LOW2_Msk       (0x1UL << CAN_TSR_LOW2_Pos)                      /*!< 0x80000000 */
#define CAN_TSR_LOW2           CAN_TSR_LOW2_Msk                                /*!<Lowest Priority Flag for Mailbox 2 */




#include "string.h"

static HAL_CAN_t CAN;


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
}

uint8_t HAL_CANToInitMode()
{

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


}
uint8_t HAL_CANToOperatingMode()
{

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

}


uint8_t HAL_CANSend(CAN_TX_FRAME_TYPE *buffer)
{

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

}

void HAL_CANSetFiters(uint8_t filter_index, uint32_t f1,uint32_t f2,uint32_t f3,uint32_t f4, HAL_CAN_FILTER_FIFO_t FIFO)
{
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

}

HAL_CAN_ERROR_t HAL_CANGetRXMessage( HAL_CAN_RX_FIFO_NUMBER_t fifo,  CAN_FRAME_TYPE * rx_message )
{
	HAL_CAN_ERROR_t res = HAL_CAN_ERROR;

    uint32_t            typeID = ((uint8_t)0x04 & (CAN1->sRxMailBox[fifo].RXMID));
    if(typeID == CAN_TYPEID_STD)
    {
    	rx_message->ident = (CAN1->sRxMailBox[fifo].RXMID >> 21) & 0x000007FF;
    	uint32_t            remoteTxReq = CAN1->sRxMailBox[fifo].RXMID_B.RFTXREQ;

    	//Игнорируем RTR фрайм, поскольку у него нет данных для нас.
    	if ( remoteTxReq != CAN_RTXR_REMOTE )
    	{
    		rx_message->DLC = CAN1->sRxMailBox[fifo].RXDLEN_B.DLCODE;
    		rx_message->filter_id = CAN1->sRxMailBox[fifo].RXDLEN_B.FMIDX;
    		/* Get the data field */
    		rx_message->data[0] = CAN1->sRxMailBox[fifo].RXMDL_B.DATABYTE0;
    		rx_message->data[1] = CAN1->sRxMailBox[fifo].RXMDL_B.DATABYTE1;
    		rx_message->data[2] = CAN1->sRxMailBox[fifo].RXMDL_B.DATABYTE2;
    		rx_message->data[3] = CAN1->sRxMailBox[fifo].RXMDL_B.DATABYTE3;
    		rx_message->data[4] = CAN1->sRxMailBox[fifo].RXMDH_B.DATABYTE4;
    		rx_message->data[5] = CAN1->sRxMailBox[fifo].RXMDH_B.DATABYTE5;
    		rx_message->data[6] = CAN1->sRxMailBox[fifo].RXMDH_B.DATABYTE6;
    		rx_message->data[7] = CAN1->sRxMailBox[fifo].RXMDH_B.DATABYTE7;
    		res = HAL_CAN_OK;
    	}
    }
    if(fifo == HAL_RX_FIFO0)
    {
        CAN1->RXF0_B.RFOM0 = BIT_SET;
    }
    else
    {
        CAN1->RXF1_B.RFOM1 = BIT_SET;
    }

   return (res);

}

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

	  if (( CAN1->TXSTS & (CAN_FLAG_REQC0 & 0x000FFFFF)) != RESET )
	  {
		  CAN1->TXSTS = CAN_FLAG_REQC0   & 0x000FFFFF;
	  }
	  if (( CAN1->TXSTS & (CAN_FLAG_REQC1 & 0x000FFFFF)) != RESET )
	  {
	    	CAN1->TXSTS = CAN_FLAG_REQC1   & 0x000FFFFF;
	  }
	  if (( CAN1->TXSTS & (CAN_FLAG_REQC2 & 0x000FFFFF)) != RESET )
	  {
		  CAN1->TXSTS = CAN_FLAG_REQC2   & 0x000FFFFF;
	  }
	  CAN1->TXSTS_B.REQCFLG0 = BIT_SET;
	  CAN1->TXSTS_B.REQCFLG1 = BIT_SET;
	  CAN1->TXSTS_B.REQCFLG2 = BIT_SET;
	  CAN.txcallback();
}
#endif
