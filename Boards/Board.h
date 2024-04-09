/*!
 * @file       Board.h
 *
 * @brief      Header file for Board
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
#ifndef BOARD_H
#define BOARD_H

/* Includes */
#if defined (APM32F407_MINI)
#include "Board_APM32F407_MINI\inc\Board_APM32F407_MINI.h"

#elif defined (APM32F407_ELE_HUETB)
#include "Board_APM32F407_ELE_HUETB\inc\Board_APM32F407_ELE_HUETB.h"

#elif defined (APM32F407_TINY)
#include "Board_APM32F407_TINY\inc\Board_APM32F407_TINY.h"

#elif defined (APM32F411_TINY)
#include "Board_APM32F411_TINY\inc\Board_APM32F411_TINY.h"

#else
#error "Please select first the APM32  board to be used (in board.h)"
#endif

/** @addtogroup Board
  @{
*/

#endif

/**@} end of group Board */
