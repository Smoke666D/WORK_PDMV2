/*
 * hal_gpio_apm32.c
 *
 *  Created on: 16 июл. 2024 г.
 *      Author: i.dymov
 */

#include "hal_gpio.h"
#if MCU == APM32


#include "apm32f4xx_rcm.h"
#include "apm32f4xx_gpio.h"


static GPIO_T * PORTS[]= { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};

static void HAL_InitRCC(PortName_t PORT );



/*
 * Инициализация прота в режиме выхода
 */
void HAL_InitGpioOut( PortName_t PORT, uint16_t Pin  )
{
	GPIO_Config_T gpioConfigStruct;
	HAL_InitRCC( PORT);
	gpioConfigStruct.mode = GPIO_MODE_OUT;
	gpioConfigStruct.pin = Pin;
	gpioConfigStruct.otype = GPIO_OTYPE_PP;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed = GPIO_SPEED_2MHz ;
	GPIO_Config(PORTS[PORT], &gpioConfigStruct);
}

/*
 * Инициализация порта в режиме аналогового входа
 */
void HAL_InitGpioAIN(PortName_t PORT, uint16_t Pin )
{

	GPIO_Config_T gpioConfigStruct;
	HAL_InitRCC( PORT);
	gpioConfigStruct.mode = GPIO_MODE_AN;
	gpioConfigStruct.pin =Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
	GPIO_Config(PORTS[PORT], &gpioConfigStruct);

}

/*
 * Инициализация порта в режиме входа с пулап
 */
void HAL_InitGpioInPUP(PortName_t PORT, uint16_t Pin)
{
	GPIO_Config_T gpioConfigStruct;
	HAL_InitRCC( PORT);
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_UP;
    GPIO_Config(PORTS[PORT], &gpioConfigStruct);
}

/*
 * Инициализация порта в режиме входа
 */
void HAL_InitGpioIn(PortName_t PORT, uint16_t Pin)
{
	GPIO_Config_T gpioConfigStruct;
	HAL_InitRCC( PORT);
	gpioConfigStruct.mode = GPIO_MODE_IN;
    gpioConfigStruct.pin = Pin;
	gpioConfigStruct.pupd = GPIO_PUPD_NOPULL;
    GPIO_Config(PORTS[PORT], &gpioConfigStruct);
}



/*
 * Иницализация порта в режиме альтернативной функции
 */
void HAL_InitGpioAF(PortName_t PORT, uint16_t Pin, uint32_t AF ,  GPIO_MODE_t mode )
{

	GPIO_PIN_SOURCE_T pin_source;
	/*switch (Pin)
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
  */
	for (pin_source = 0; pin_source <= GPIO_PIN_SOURCE_15; pin_source++ )
	{
		if (Pin & 1<<pin_source) break;
	}

	GPIO_Config_T gpioConfigStruct;
	HAL_InitRCC( PORT);
	GPIO_ConfigPinAF(PORTS[PORT] ,  pin_source , AF);
	gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = Pin;
    gpioConfigStruct.otype = mode;
	gpioConfigStruct.pupd  =  GPIO_PUPD_NOPULL;
	gpioConfigStruct.speed =  GPIO_SPEED_100MHz  ;
    GPIO_Config(PORTS[PORT], &gpioConfigStruct);
}

BitState_t HAL_GetBit( PortName_t  port, uint16_t pin)
{
	return ((PORTS[port]->IDATA & pin) ?  BIT_SET : BIT_RESET);
}

/*
 * Функция установки бита порта
*/

void HAL_SetBit(  PortName_t  port, uint16_t pin )
{
	PORTS[port]->BSCL = pin;
}



/*
 * Получить бит порта
 */
BitState_t xHAL_GetOutBit( PortName_t  port, uint16_t pin)
{
	return ((PORTS[port]->ODATA & pin) ? BIT_SET : BIT_RESET);
}

/*
 * Функция сброса бита порта
 */
 void HAL_ResetBit(  PortName_t  port, uint16_t pin )
{
	PORTS[port]->BSCH = pin;
}


/*
 * Функция инициализации тактирования указанного порта
 */

static const u32 RCM_MASK[]= {RCM_AHB1_PERIPH_GPIOA,RCM_AHB1_PERIPH_GPIOB,RCM_AHB1_PERIPH_GPIOC,RCM_AHB1_PERIPH_GPIOD,RCM_AHB1_PERIPH_GPIOE,
			 RCM_AHB1_PERIPH_GPIOF,RCM_AHB1_PERIPH_GPIOH,RCM_AHB1_PERIPH_GPIOG};

static void HAL_InitRCC(PortName_t PORT )
{
	RCM->AHB1CLKEN |= RCM_MASK[PORT];

}

#endif
