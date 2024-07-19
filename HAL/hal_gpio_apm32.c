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

#include "string.h"

/*
 * Инициализация прота в режиме выхода
 */
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  )
{
	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_OUT;
	gpioConfigStruct.pin = Pin;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
	GPIO_Config(PORT, &gpioConfigStruct);
}

/*
 * Инициализация порта в режиме аналогового входа
 */
void HAL_InitGpioAIN(PortName_t PORT, uint16_t Pin )
{
	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_AN;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	GPIO_Config(PORT, &gpioConfigStruct);
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
    GPIO_Config(PORT, &gpioConfigStruct);
}



/*
 * Инициализация порта в режиме входа
 */
void HAL_InitGpioIn(PortName_t PORT, uint16_t Pin)
{
	GPIO_Config_T gpioConfigStruct;
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
    GPIO_Config(PORT, &gpioConfigStruct);
}



/*
 * Иницализация порта в режиме альтернативной функции
 */
void HAL_InitGpioAF(PortName_t PORT, uint16_t Pin, uint32_t AF ,  GPIO_MODE_t mode )
{

	GPIO_PIN_SOURCE_T pin_source;

	for (pin_source =0; pin_source <16; pin_source ++)
	{
		if (Pin & 0x01<<pin_source) break;
	}
	GPIO_Config_T gpioConfigStruct;
	GPIO_ConfigPinAF(PORT ,  pin_source , AF);
	gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = Pin;
    gpioConfigStruct.otype = mode;//GPIO_OTYPE_PP;
	gpioConfigStruct.pupd  =  GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_100MHz  ;
    GPIO_Config(PORT, &gpioConfigStruct);
}
/*
 * Получить бит
 */
BitState_t HAL_GetBit( PortName_t  port, uint16_t pin)
{
	return ((port->IDATA & pin) ?  BIT_SET : BIT_RESET);
}

/*
 * Функция установки бита порта
 */
void HAL_SetBit(  PortName_t  port, uint16_t pin )
{
	port->BSCL = pin;
}



/*
 * Получить бит порта
 */
BitState_t xHAL_GetOutBit( PortName_t  port, uint16_t pin)
{
	return ((port->ODATA & pin) ? BIT_SET : BIT_RESET);
}

/*
 * Функция сброса бита порта
 */
void HAL_ResetBit(  PortName_t  port, uint16_t pin )
{
	port->BSCH = pin;
}
/*
 * Функция инициализации тактирования указанного порта
 */
 void HAL_InitRCC(PortName_t PORT )
{
	if (PORT ==  PORT_A)
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA);
	else
	if (PORT ==  PORT_B)

			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB);
	else
	if (PORT ==  PORT_C)

			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC);
	else
	if (PORT ==  PORT_D)
			RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOD);
	else
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

}

#define GPIO_PORT_COUNT  7
static const RCM_AHB1_PERIPH_T PERIPH_MASK[GPIO_PORT_COUNT] = { RCM_AHB1_PERIPH_GPIOA,RCM_AHB1_PERIPH_GPIOB,RCM_AHB1_PERIPH_GPIOC,RCM_AHB1_PERIPH_GPIOD,RCM_AHB1_PERIPH_GPIOG,
		 RCM_AHB1_PERIPH_GPIOE,RCM_AHB1_PERIPH_GPIOF};


 void HAL_InitGPO()
 {
	 for (u8 i=0;i< GPIO_PORT_COUNT;i++)
	 {
		 RCM->AHB1CLKEN |= PERIPH_MASK[i];
		 RCM->AHB1RST |= PERIPH_MASK[i];
		 RCM->AHB1RST &= (uint32_t)~PERIPH_MASK[i];
	 }
 }

#endif
