/*
 * hal_gpio.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "hal_gpio.h"
#if MCU == APM32
	#include "apm32f4xx_rcm.h"
	#include "apm32f4xx_gpio.h"
#endif
#if MCU == CH32V2
	#include "ch32v20x_rcc.h"
	#include "ch32v20x_gpio.h"
#endif
#include "string.h"

static void HAL_InitRCC(PortName_t PORT );

#if MCU == CH32V2


void HAL_InitGPIO(GPIO_TypeDef *GPIOx,    uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed,
    GPIOMode_TypeDef GPIO_Mode)
{
    uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
    uint32_t tmpreg = 0x00, pinmask = 0x00;

    currentmode = ((uint32_t)GPIO_Mode) & ((uint32_t)0x0F);

    if((((uint32_t)GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
    {
        currentmode |= (uint32_t)GPIO_Speed;
    }
#if defined(PORT_C_ENABLE) || defined(DEFAULT_C)
#if defined (CH32V20x_D6)

    if((GPIOx == GPIOC) && (((*(uint32_t *) 0x40022030) & 0x0F000000) == 0)){
        GPIO_Pin = GPIO_Pin >> 13;
    }

#endif
#endif


    if(((uint32_t)GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
    {
        tmpreg = GPIOx->CFGLR;
        for(pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = ((uint32_t)0x01) << pinpos;
            currentpin = (GPIO_Pin) & pos;
            if(currentpin == pos)
            {
                pos = pinpos << 2;
                pinmask = ((uint32_t)0x0F) << pos;
                tmpreg &= ~pinmask;
                tmpreg |= (currentmode << pos);
                if(GPIO_Mode == GPIO_Mode_IPD)
                {
                    GPIOx->BCR = (((uint32_t)0x01) << pinpos);
                }
                else
                {
                    if(GPIO_Mode == GPIO_Mode_IPU)
                    {
                        GPIOx->BSHR = (((uint32_t)0x01) << pinpos);
                    }
                }
            }
        }
        GPIOx->CFGLR = tmpreg;
    }

    if(GPIO_Pin > 0x00FF)
    {
        tmpreg = GPIOx->CFGHR;

        for(pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = (((uint32_t)0x01) << (pinpos + 0x08));
            currentpin = ((GPIO_Pin) & pos);
            if(currentpin == pos)
            {
                pos = pinpos << 2;
                pinmask = ((uint32_t)0x0F) << pos;
                tmpreg &= ~pinmask;
                tmpreg |= (currentmode << pos);
                if(GPIO_Mode == GPIO_Mode_IPD)
                {
                    GPIOx->BCR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
                if(GPIO_Mode == GPIO_Mode_IPU)
                {
                    GPIOx->BSHR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
            }
        }
        GPIOx->CFGHR = tmpreg;
    }
}

#endif
/*
 * API Функция иницилазиации тактирования порта. Сохраняет флаг, что бы не было повторонй инициализации
 */
/*
 * Инициализация прота в режиме выхода
 */
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  )
{
#if MCU == APM32
	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2
#endif
	 HAL_InitRCC( PORT);
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_OUT;
	gpioConfigStruct.pin = Pin;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
	GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
	HAL_InitGPIO( PORT, Pin,GPIO_Speed_2MHz, GPIO_Mode_Out_PP );
#endif
}

/*
 * Инициализация порта в режиме аналогового входа
 */
void HAL_InitGpioAIN(PortName_t PORT, uint16_t Pin )
{
#if MCU == APM32
	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2
#endif
	 HAL_InitRCC( PORT);
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_AN;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
    HAL_InitGPIO( PORT, Pin,GPIO_Speed_50MHz, GPIO_Mode_AIN );
#endif
}

/*
 * Инициализация порта в режиме входа с пулап
 */
void HAL_InitGpioInPUP(PortName_t PORT, uint16_t Pin)
{
#if MCU == APM32
	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2
#endif
	 HAL_InitRCC( PORT);
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_UP;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
    HAL_InitGPIO( PORT, Pin,GPIO_Speed_50MHz, GPIO_Mode_IPU );
#endif
}



/*
 * Инициализация порта в режиме входа
 */
void HAL_InitGpioIn(PortName_t PORT, uint16_t Pin)
{
#if MCU == APM32
	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2

#endif
	 HAL_InitRCC( PORT);
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
    HAL_InitGPIO( PORT, Pin,GPIO_Speed_50MHz,GPIO_Mode_IN_FLOATING );
#endif
}



/*
 * Иницализация порта в режиме альтернативной функции
 */
void HAL_InitGpioAF(PortName_t PORT, uint16_t Pin, uint32_t AF ,  GPIO_MODE_t mode )
{
#if MCU == APM32
	GPIO_PIN_SOURCE_T pin_source;
	switch (Pin)
	{
		default:
		case GPIO_PIN_0:
			pin_source = GPIO_PIN_SOURCE_0;
			break;
		case GPIO_PIN_1:
			pin_source = GPIO_PIN_SOURCE_1;
			break;
		case GPIO_PIN_2:
			pin_source = GPIO_PIN_SOURCE_2;
			break;
		case GPIO_PIN_3:
			pin_source = GPIO_PIN_SOURCE_3;
			break;
		case GPIO_PIN_4:
		   pin_source = GPIO_PIN_SOURCE_4;
			break;
		case GPIO_PIN_5:
			pin_source = GPIO_PIN_SOURCE_5;
			break;
		case GPIO_PIN_6:
			pin_source = GPIO_PIN_SOURCE_6;
			break;
	    case GPIO_PIN_7:
			pin_source = GPIO_PIN_SOURCE_7;
			break;
	    case GPIO_PIN_8:
	    	pin_source = GPIO_PIN_SOURCE_8;
	    	break;
	    case GPIO_PIN_9:
	    	pin_source = GPIO_PIN_SOURCE_9;
	    	break;
	    case GPIO_PIN_10:
	    	pin_source = GPIO_PIN_SOURCE_10;
	    	break;
	    case GPIO_PIN_11:
	    	pin_source = GPIO_PIN_SOURCE_11;
	    	break;
	    case GPIO_PIN_12:
	    	pin_source = GPIO_PIN_SOURCE_12;
	    	break;
	    case GPIO_PIN_13:
	    	pin_source = GPIO_PIN_SOURCE_13;
	    	break;
	    case GPIO_PIN_14:
	   	    pin_source = GPIO_PIN_SOURCE_14;
	   	    break;
	   case GPIO_PIN_15:
	   	    pin_source = GPIO_PIN_SOURCE_15;
	   	    break;
	}

	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2
#endif
	 HAL_InitRCC( PORT);
#if MCU == APM32
	GPIO_ConfigPinAF(PORT ,  pin_source , AF);
	gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = Pin;
    gpioConfigStruct.otype = mode;//GPIO_OTYPE_PP;
	gpioConfigStruct.pupd  = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_50MHz  ;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
    if (AF != 0 )
    {
        RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
    	GPIO_PinRemapConfig(AF,ENABLE);
    }
    HAL_InitGPIO( PORT, Pin,GPIO_Speed_50MHz,mode );
#endif

}
/*
 * Получить бит
 */
BitState_t HAL_GetBit( PortName_t  port, uint16_t pin)
{
#if MCU == APM32
    return (GPIO_ReadInputBit( port , pin));
#endif
#if MCU == CH32V2
#ifdef PORT_C_ENABLE
    return ( GPIO_ReadInputDataBit(port, pin));
#else
    return (( port->INDR & pin))? (uint8_t)Bit_SET : (uint8_t)Bit_RESET;
#endif
#endif
}

/*
 * Функция установки бита порта
 */
void HAL_SetBit(  PortName_t  port, uint16_t pin )
{
#if MCU == APM32
    GPIO_SetBit(port, pin);
#endif
#if MCU == CH32V2
#ifdef PORT_C_ENABLE
    GPIO_SetBits(port, pin);
#else
     port->BSHR = pin;
#endif
#endif
}

/*
 * Получить бит порта
 */
BitState_t xHAL_GetOutBit( PortName_t  port, uint16_t pin)
{
#if MCU == APM32
    return (GPIO_ReadOutputBit( port , pin));
#endif
#if MCU == CH32V2
#ifdef PORT_C_ENABLE
    return ( GPIO_ReadOutputDataBit(port, pin));
#else
    return ( (port->OUTDR & pin) )? (uint8_t)Bit_SET :  (uint8_t)Bit_RESET;
#endif
#endif
}

/*
 * Функция сброса бита порта
 */
void HAL_ResetBit(  PortName_t  port, uint16_t pin )
{
#if MCU == APM32
    GPIO_ResetBit(port, pin);
#endif
#if MCU == CH32V2
#ifdef PORT_C_ENABLE
    GPIO_ResetBits(port, pin);
#else
    port->BCR = pin;
#endif
#endif
}
/*
 * Функция инициализации тактирования указанного порта
 */
static void HAL_InitRCC(PortName_t PORT )
{
    if (PORT ==  PORT_A)
#if MCU == APM32
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA);
#endif
#if MCU == CH32V2
        RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
#endif
    else
    if (PORT ==  PORT_B)
#if MCU == APM32
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB);
#endif
#if MCU == CH32V2
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOB;

#endif
    else
    if (PORT ==  PORT_C)
#if MCU == APM32
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC);
#endif
#if MCU == CH32V2
#ifdef PORT_C_ENALBE
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
#endif
#ifndef PORT_C_ENABLE
    {}
#endif
#endif
#if MCU == APM32
    else
    if (PORT ==  PORT_D)
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOD);
    if (PORT ==  PORT_E)
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOE);
            else
    if (PORT ==  PORT_F)
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOF);
    else
    if (PORT ==  PORT_H)
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOH);
    else
    if (PORT ==  PORT_G)
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOG);
#endif
}

