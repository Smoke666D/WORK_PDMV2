/*
 * hal_rtc_ch32.c
 *
 *  Created on: 1 авг. 2024 г.
 *      Author: i.dymov
 */

#include "hal_rtc.h"

#if MCU == CH32V2

#include "hal_irq.h"
/* RTC_Private_Defines */
#define RTC_LSB_MASK     ((uint32_t)0x0000FFFF) /* RTC LSB Mask */
#define PRLH_MSB_MASK    ((uint32_t)0x000F0000) /* RTC Prescaler MSB Mask */
void RTC_IRQHandler ( void )  __attribute__((interrupt()));


//Callbacl функция для вызова из секундрого прерывания RTC
static void (* func)( void);



void HAL_RTC_IT_Init(  void (* rtc_it_callback) ( void ), uint8_t prior, uint8_t subprior )
{

    uint8_t temp = 0;
    RCC->APB1PCENR |=(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP);   //Разрешаем тактирование
    PWR->CTLR |= (1 << 8);
    RCC->BDCTLR |= (1<<16);    //Сборс модуля Buckup
    RCC->BDCTLR &= ~(1<<16);

    RCC_LSEConfig(RCC_LSE_ON);

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
    {
              temp++;
              Delay_Ms(20);
    }
    if(temp >= 250) return;
    RCC->BDCTLR |=RCC_RTCCLKSource_LSE;
    RCC->BDCTLR |= (1<<15); //This function must be used only after the RTC clock was selected
                            // using the RCC_RTCCLKConfig function.
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    RTC->CTLRH |= RTC_IT_SEC;   //Разрешаем прерывание
    RTC_WaitForLastTask();
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();
    PFIC_IRQ_ENABLE_PG1(RTC_IRQn,prior,subprior);

	  func = rtc_it_callback;
	  return;
}


void RTC_IRQHandler ( void )
{
    if ((RTC->CTLRH & RTC_FLAG_SEC) )
    {
        func();
        RTC->CTLRL &= (uint16_t)~RTC_FLAG_SEC;
    }
    RTC_WaitForLastTask();
    return;

}



#endif
