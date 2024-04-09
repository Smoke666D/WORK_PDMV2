/*
 * hal_gpio.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */



#include "hal_gpio.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx_gpio.h"
#include "string.h"

PortInitState_t InitPort[PORT_H +1] = {0};
GPIO_T * Ports[PORT_H +1]  = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH};


static void HAL_InitRCC(PortName_t PORT );

void HAL_InitGpioLib()
{
	memset(InitPort,0x00, PORT_H + 1 );
}

/*
 * API Функция иницилазиации тактирования порта. Сохраняет флаг, что бы не было повторонй инициализации
 */
void HAL_InitGpioRCC( PortName_t PORT  )
{
    if  (InitPort[ PORT]==PORT_NOTINIT)
    {
    	HAL_InitRCC( PORT);
    	InitPort[PORT] = PORT_INIT;
    }
	return;
}

/*
 * Инициализация прота в режиме выхода
 */
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  )
{
	GPIO_Config_T gpioConfigStruct;
	HAL_InitGpioRCC(PORT );
	gpioConfigStruct.mode = GPIO_MODE_OUT;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
	GPIO_Config(Ports[PORT], &gpioConfigStruct);
}

/*
 * Инициализация порта в режиме аналогового входа
 */
void HAL_InitGpioAIN(PortName_t PORT, uint16_t Pin )
{
	GPIO_Config_T gpioConfigStruct;
	HAL_InitGpioRCC(PORT );
	gpioConfigStruct.mode = GPIO_MODE_AN;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	GPIO_Config(Ports[PORT], &gpioConfigStruct);
}

/*
 * Инициализация порта в режиме входа с пулап
 */
void HAL_InitGpioInPUP(PortName_t PORT, uint16_t Pin)
{
	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_UP;
    GPIO_Config(Ports[PORT], &gpioConfigStruct);
}
/*
 * Иницализация порта в режиме альтернативной функции
 */
void HAL_InitGpioAF(PortName_t PORT, uint16_t Pin, uint16_t AF )
{

	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = Pin;
    gpioConfigStruct.otype = GPIO_OTYPE_OD;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
    GPIO_Config(Ports[PORT], &gpioConfigStruct);
    GPIO_ConfigPinAF(Ports[PORT] , Pin, AF);
}
/*
 * Получить бит порта
 */
BitState_t xHAL_GetBit( PortName_t  port, uint16_t pin)
{
	return (GPIO_ReadOutputBit( Ports[port] , pin));
}

/*
 * Функция установки бита порта
 */
void vHAL_SetBit(  PortName_t  port, uint16_t pin )
{

	GPIO_SetBit(Ports[port], pin);
//	GPIO_WriteBitValue(out[out_name].GPIOx, out[out_name].GPIO_Pin , CS_ENABLE);

}
/*
 * Функция сброса бита порта
 */
void vHAL_ResetBit(  PortName_t  port, uint16_t pin )
{
	GPIO_ResetBit(Ports[port], pin);
}
/*
 * Функция инициализации тактирования указанного порта
 */
static void HAL_InitRCC(PortName_t PORT )
{
	switch ( PORT)
	{
		case PORT_A:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA);
			break;
		case PORT_B:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB);
			break;
		case PORT_C:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC);
			break;
		case PORT_D:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOD);
			break;
		case PORT_E:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOE);
			break;
		case PORT_F:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOF);
			break;
		case PORT_H:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOH);
			break;
		case PORT_G:
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOG);
			break;
		default:
			break;
	}
}
