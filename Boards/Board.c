/*!
 * @file       Board.c
 *
 * @brief      This file provides firmware functions to manage Leds and push-buttons
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

/* Includes */
#include "Board.h"

#if defined (APM32F407_MINI)
#include "Board_APM32F407_MINI\src\Board_APM32F407_MINI.c"

#elif defined (APM32F407_ELE_HUETB)
#include "Board_APM32F407_ELE_HUETB\src\Board_APM32F407_ELE_HUETB.c"

#elif defined (APM32F407_TINY)
#include "Board_APM32F407_TINY\src\Board_APM32F407_TINY.c"

#elif defined (APM32F411_TINY)
#include "Board_APM32F411_TINY\src\Board_APM32F411_TINY.c"

#else
#error "Please select first the APM32 board to be used (in board.c)"
#endif

/** @addtogroup Board
  @{

*/

/**@} end of group Board */
