/*!
 * @file       readme.txt
 *
 * @brief      This file is routine instruction
 *
 * @version     V1.0.3
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

&par Example Description

This example describes show how to how to use FreeRTOS create multiple tasks.

Usart test task : USART1 and USART2 send or received data to each other. Verification will occur after transmission,
if send and received data pass, LED3 will be on all the time.
if send and received data fault, LED3 will be off all the time.
if send or received data fault, LED3 will be constantly flickered alternately.

Led toggle task : The IO of LED2 is configed to toggle constantly

The phenomenon of LED2 constantly flickered alternately,
if Data transmission pass, LED3 will be on, and data interaction process can be displayed using serial assistant.

&par Hardware Description

USART1_TX(PA9) ----> USART2_RX(PA3)
USART1_RX(PA10)----> USART2_TX(PA2)

  - USART1 and USART2 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - Examples/src/main.c                              Main program
  - Examples/src//apm32f4xx_int.c              Interrupt handlers
  - Examples/inc/FreeRTOSConfig.h            FreeRTOS config functions

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V8.50.5.26295

&par Hardware and Software environment

  - This example runs on APM32F407 MINI Devices.
  - This example runs on APM32F411 TINY Devices.