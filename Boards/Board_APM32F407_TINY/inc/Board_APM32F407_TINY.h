/*!
 * @file        Board_APM32F407_TINY.h
 *
 * @brief       This file contains definitions for APM32F407_TINY's Leds, push-buttons hardware resources
 *
 * @version     V1.0.2
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2021-2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Define to prevent recursive inclusion */
#ifndef __BOARD_APM32F407_TINY_H
#define __BOARD_APM32F407_TINY_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes */
#include "apm32f4xx.h"
#include "apm32f4xx_gpio.h"
#include "apm32f4xx_rcm.h"
#include "apm32f4xx_eint.h"
#include "apm32f4xx_usart.h"
#include "apm32f4xx_misc.h"
#include "apm32f4xx_syscfg.h"
#include "apm32f4xx_dma.h"
#include "apm32f4Xx_sdio.h"
#include "apm32f4xx_dci.h"

/** @addtogroup Board
  @{
*/

/** @addtogroup Board_APM32F407_TINY
  @{
*/

/** @defgroup APM32F407_TINY_Enumerations
  @{
  */

typedef enum
{
    LED2 = 0,
    LED3 = 1
} Led_TypeDef;

typedef enum
{
    BUTTON_KEY1 = 0,
    BUTTON_KEY2 = 1
} Button_TypeDef;

typedef enum
{
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EINT = 1
} ButtonMode_TypeDef;

typedef enum
{
    COM1 = 0,
    COM2 = 1
} COM_TypeDef;

/**
 * @brief   gpio port source define
 */
typedef enum
{
   GPIO_PORT_SOURCE_A,
   GPIO_PORT_SOURCE_B,
   GPIO_PORT_SOURCE_C,
   GPIO_PORT_SOURCE_D,
   GPIO_PORT_SOURCE_E,
   GPIO_PORT_SOURCE_F,
   GPIO_PORT_SOURCE_G,
} GPIO_PORT_SOURCE_T;

/**
  * @}
  */

/** @addtogroup APM32F407_TINY_LED
  @{
  */
#define LEDn                             2

#define LED2_PIN                         GPIO_PIN_6
#define LED2_GPIO_PORT                   GPIOE
#define LED2_GPIO_CLK                    RCM_AHB1_PERIPH_GPIOE

#define LED3_PIN                         GPIO_PIN_5
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK                    RCM_AHB1_PERIPH_GPIOE

/**
  * @}
  */

/** @addtogroup APM32F407_TINY_BUTTON
  @{
  */
#define BUTTONn                          2

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                   GPIO_PIN_10
#define KEY1_BUTTON_GPIO_PORT             GPIOC
#define KEY1_BUTTON_GPIO_CLK              RCM_AHB1_PERIPH_GPIOC
#define KEY1_BUTTON_EINT_LINE             EINT_LINE_10
#define KEY1_BUTTON_EINT_PORT_SOURCE      SYSCFG_PORT_GPIOC
#define KEY1_BUTTON_EINT_PIN_SOURCE       SYSCFG_PIN_10
#define KEY1_BUTTON_EINT_IRQn             EINT15_10_IRQn
/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                   GPIO_PIN_11
#define KEY2_BUTTON_GPIO_PORT             GPIOC
#define KEY2_BUTTON_GPIO_CLK              RCM_AHB1_PERIPH_GPIOC
#define KEY2_BUTTON_EINT_LINE             EINT_LINE_11
#define KEY2_BUTTON_EINT_PORT_SOURCE      SYSCFG_PORT_GPIOC
#define KEY2_BUTTON_EINT_PIN_SOURCE       SYSCFG_PIN_11
#define KEY2_BUTTON_EINT_IRQn             EINT15_10_IRQn

/**
  * @}
  */

/** @addtogroup APM32F407_TINY_COM
  @{
  */
#define COMn                            2

/**
 * @brief Definition for COM port1, connected to USART1
 */
#define TINY_COM1                        USART1
#define TINY_COM1_CLK                    RCM_APB2_PERIPH_USART1
#define TINY_COM1_TX_PIN                 GPIO_PIN_9
#define TINY_COM1_TX_GPIO_PORT           GPIOA
#define TINY_COM1_TX_GPIO_CLK            RCM_AHB1_PERIPH_GPIOA
#define TINY_COM1_TX_PIN_SOURCE          GPIO_PIN_SOURCE_9
#define TINY_COM1_RX_PIN                 GPIO_PIN_10
#define TINY_COM1_RX_GPIO_PORT           GPIOA
#define TINY_COM1_RX_GPIO_CLK            RCM_AHB1_PERIPH_GPIOA
#define TINY_COM1_RX_PIN_SOURCE          GPIO_PIN_SOURCE_10
#define TINY_COM1_IRQn                   USART1_IRQn
#define TINY_COM1_GPIO_AF                GPIO_AF_USART1

/**
 * @brief Definition for COM port2, connected to USART2
 */
#define TINY_COM2                        USART2
#define TINY_COM2_CLK                    RCM_APB1_PERIPH_USART2
#define TINY_COM2_TX_PIN                 GPIO_PIN_2
#define TINY_COM2_TX_GPIO_PORT           GPIOA
#define TINY_COM2_TX_PIN_SOURCE          GPIO_PIN_SOURCE_2
#define TINY_COM2_TX_GPIO_CLK            RCM_AHB1_PERIPH_GPIOA
#define TINY_COM2_RX_PIN                 GPIO_PIN_3
#define TINY_COM2_RX_GPIO_PORT           GPIOA
#define TINY_COM2_RX_GPIO_CLK            RCM_AHB1_PERIPH_GPIOA
#define TINY_COM2_RX_PIN_SOURCE          GPIO_PIN_SOURCE_3
#define TINY_COM2_IRQn                   USART2_IRQn
#define TINY_COM2_GPIO_AF                GPIO_AF_USART2

/**
  * @}
  */

/** @defgroup APM32F407_TINY_Fuctions
  @{
  */
void APM_TINY_LEDInit(Led_TypeDef Led);
void APM_TINY_LEDOn(Led_TypeDef Led);
void APM_TINY_LEDOff(Led_TypeDef Led);
void APM_TINY_LEDToggle(Led_TypeDef Led);

void APM_TINY_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t APM_TINY_PBGetState(Button_TypeDef Button);

void APM_TINY_COMInit(COM_TypeDef COM, USART_Config_T* configStruct);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_APM32F407_TINY_H */

/**@} end of group Board_APM32F407_TINY */
/**@} end of group Board */
