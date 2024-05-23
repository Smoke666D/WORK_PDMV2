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

PortInitState_t InitPort[MAX_GPIO_PORT +1] = {0};


static void HAL_InitRCC(PortName_t PORT );

void HAL_InitGpioLib()
{
	memset(InitPort,0x00, MAX_GPIO_PORT + 1 );
}

/*
 * API Функция иницилазиации тактирования порта. Сохраняет флаг, что бы не было повторонй инициализации
 */
void HAL_InitGpioRCC( PortName_t PORT  )
{

	uint8_t port_index = 0;
    if ( PORT == PORT_B )  port_index = 1;
    else
    if ( PORT == PORT_C )  port_index = 2;
    else
    if ( PORT == PORT_D )  port_index = 3;
    else
    if ( PORT == PORT_E )  port_index = 4;
    else
    if ( PORT == PORT_F )  port_index = 5;
    else
    if ( PORT == PORT_G )  port_index = 6;
    if  (InitPort[port_index]==PORT_NOTINIT)
    {
        HAL_InitRCC( PORT);
        InitPort[port_index ] = PORT_INIT;
    }

	return;
}



/*
 * Инициализация прота в режиме выхода
 */
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  )
{
#if MCU == APM32
	GPIO_Config_T gpioConfigStruct;
#endif
#if MCU == CH32V2
	GPIO_InitTypeDef gpioConfigStruct;
#endif
	HAL_InitGpioRCC( PORT );
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_OUT;
	gpioConfigStruct.pin = Pin;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
	GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
	gpioConfigStruct.GPIO_Pin =  Pin;
	gpioConfigStruct.GPIO_Mode =GPIO_Mode_Out_PP ;
	gpioConfigStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( PORT, &gpioConfigStruct);
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
	GPIO_InitTypeDef gpioConfigStruct;
#endif
	HAL_InitGpioRCC(PORT );
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_AN;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
	gpioConfigStruct.GPIO_Pin =  Pin;
	gpioConfigStruct.GPIO_Mode =GPIO_Mode_AIN ;
	gpioConfigStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( PORT, &gpioConfigStruct);
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
	GPIO_InitTypeDef gpioConfigStruct;
#endif
	HAL_InitGpioRCC(PORT );
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_UP;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
	gpioConfigStruct.GPIO_Pin   =  Pin;
	gpioConfigStruct.GPIO_Mode  = GPIO_Mode_IPU;
	gpioConfigStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( PORT, &gpioConfigStruct);
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
	GPIO_InitTypeDef gpioConfigStruct;
#endif
	HAL_InitGpioRCC(PORT );
#if MCU == APM32
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
	gpioConfigStruct.GPIO_Pin   =  Pin;
	gpioConfigStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
	gpioConfigStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( PORT, &gpioConfigStruct);
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
	GPIO_InitTypeDef gpioConfigStruct;
#endif
	HAL_InitGpioRCC(PORT );
#if MCU == APM32
	GPIO_ConfigPinAF(PORT ,  pin_source , AF);
	gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = Pin;
    gpioConfigStruct.otype = mode;//GPIO_OTYPE_PP;
	gpioConfigStruct.pupd  =  GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_100MHz  ;
    GPIO_Config(PORT, &gpioConfigStruct);
#endif
#if MCU == CH32V2
    if (AF != 0 )
    {
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    	GPIO_PinRemapConfig(AF,ENABLE);
    }
    gpioConfigStruct.GPIO_Pin   =  Pin;
    gpioConfigStruct.GPIO_Mode  = mode;
    gpioConfigStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( PORT, &gpioConfigStruct);
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
	return ( GPIO_ReadInputDataBit(port, pin));
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
	GPIO_SetBits(port, pin);
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
	return ( GPIO_ReadOutputDataBit(port, pin));
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
	GPIO_ResetBits(port, pin);
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
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#endif
	else
	if (PORT ==  PORT_B)
#if MCU == APM32
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB);
#endif
#if MCU == CH32V2
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#endif
	else
	if (PORT ==  PORT_C)
#if MCU == APM32
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC);
#endif
#if MCU == CH32V2
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
#endif
	else
	if (PORT ==  PORT_D)
#if MCU == APM32
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOD);
#endif
#if MCU == CH32V2
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
#endif
	else
	if (PORT ==  PORT_E)
#if MCU == APM32
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOE);
#endif
#if MCU == CH32V2
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
#endif
#if MCU == APM32
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
