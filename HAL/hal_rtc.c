/*
 * hal_rtc.c
 *
 *  Created on: 26 апр. 2024 г.
 *      Author: i.dymov
 */
#include "hal_rtc.h"

#if MCU == APM32
	#include "apm32f4xx.h"
	#include "apm32f4xx_rtc.h"
	#include "apm32f4xx_pmu.h"
	#include "apm32f4xx_rcm.h"



//Callbacl функция для вызова из секундрого прерывания RTC

void vRTCInit()
{

	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
	PMU_EnableBackupAccess();
	RCM_EnableLSI();
	RTC_Reset();
	RTC_Config_T Struct;
	RCM_EnableLSI();
	while(RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);
	RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
	RCM_EnableRTCCLK();
	RTC_DisableWriteProtection();
	RTC_WaitForSynchro();
	RTC_ConfigStructInit(&Struct);
	RTC_Config(&Struct);

}

static uint8_t RTC_ByteConBcd2(uint8_t val)
{
    uint8_t bcdhigh = 0;

    while (val >= 10)
    {
        bcdhigh++;
        val -= 10;
    }

    return  ((uint8_t)(bcdhigh << 4) | val);
}
static uint8_t RTC_Bcd2ConByte(uint8_t val)
{
    uint8_t tmp = 0;
    tmp = ((uint8_t)(val & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (val & (uint8_t)0x0F));
}



void HAL_RTC_ReadTime( HAL_TimeConfig_T* time)
{
    uint32_t temp = (uint32_t)((RTC->TIME) & 0x007F7F7F);
    time->hours   = (uint8_t)((temp & 0x003F0000) >> 16);
    time->minutes = (uint8_t)((temp & 0x00007F00) >>8);
    time->seconds = (uint8_t)(temp & 0x0000007F);
    time->hours   = (uint8_t)RTC_Bcd2ConByte(time->hours);
    time->minutes = (uint8_t)RTC_Bcd2ConByte(time->minutes);
    time->seconds = (uint8_t)RTC_Bcd2ConByte(time->seconds);
}

void HAL_RTC_ReadDate(HAL_DateConfig_T* date)
{
    uint32_t temp = 0;
    temp = (uint32_t)((RTC->DATE) & 0x00FFFF3F);
    date->year  = (uint8_t)((temp & 0x00FF0000) >> 16);
    date->month = (RTC_MONTH_T)((temp & 0x00001F00) >>8);
    date->date  = (uint8_t)(temp &  0x0000003F);
    date->weekday =(RTC_WEEKDAY_T)((temp & 0x0000E000) >> 13);
    date->year  = (uint8_t)RTC_Bcd2ConByte(date->year);
    date->month = (RTC_MONTH_T)RTC_Bcd2ConByte(date->month);
    date->date  = (uint8_t)RTC_Bcd2ConByte(date->date);
    date->weekday = (RTC_WEEKDAY_T)(date->weekday);

}


uint8_t HAL_RTC_ConfigTime( HAL_TimeConfig_T* timeConfig)
{
    uint8_t state = ERROR;
    uint32_t temp = 0;

    if (RTC->CTRL_B.TIMEFCFG == BIT_RESET)
    {
        timeConfig->h12 = RTC_H12_AM;
    }

    /* Combine parameters of time */


        temp = (uint32_t)(((uint32_t)RTC_ByteConBcd2(timeConfig->hours) << 16) | \
                          ((uint32_t)RTC_ByteConBcd2(timeConfig->minutes) << 8) | \
                          ((uint32_t)RTC_ByteConBcd2(timeConfig->seconds))| \
                          (((uint32_t)(timeConfig->h12) << 22)));


    RTC_DisableWriteProtection();

    if (RTC_EnableInit() == ERROR)
    {
        state = ERROR;
    }
    else
    {
        RTC->TIME = (uint32_t)(temp & 0x007F7F7F);
        RTC->STS_B.INITEN = BIT_RESET;  //disable init

        if (RTC->CTRL_B.RCMCFG == RESET)
        {
            if (RTC_WaitForSynchro() == ERROR)
            {
                state = ERROR;
            }
            else
            {
                state = SUCCESS;
            }
        }
        else
        {
            state = SUCCESS;
        }
    }

    RTC->WRPROT = 0xFF; //Enable the write protection for RTC registers

    return state;
}


uint8_t HAL_RTC_ConfigDate( HAL_DateConfig_T* dateConfig)
{
    uint8_t state = ERROR;
    uint32_t temp = 0;

    if (  ((dateConfig->month & 0x10) == 0x10))
    {
        dateConfig->month = (RTC_MONTH_T)((dateConfig->month & (uint32_t)~(0x10)) + 0x0A);
    }


        temp = (((uint32_t)RTC_ByteConBcd2(dateConfig->year) << 16) | \
                ((uint32_t)RTC_ByteConBcd2(dateConfig->month) << 8) | \
                ((uint32_t)RTC_ByteConBcd2(dateConfig->date))| \
                ((uint32_t)(dateConfig->weekday) << 13));

    RTC_DisableWriteProtection();

    if (RTC_EnableInit() == ERROR)
    {
        state = ERROR;
    }
    else
    {
        RTC->DATE = (uint32_t)(temp & 0x00FFFF3F);
        RTC->STS_B.INITEN = BIT_RESET;  //disable init

        if (RTC->CTRL_B.RCMCFG == RESET)
        {
            if (RTC_WaitForSynchro() == ERROR)
            {
                state = ERROR;
            }
            else
            {
                state = SUCCESS;
            }
        }
        else
        {
            state = SUCCESS;
        }
    }

    RTC->WRPROT = 0xFF; //Enable the write protection for RTC registers
    return state;
}
#endif
