/**
 * @file        log_printf.c
 *
 * @brief       This file provides log function
 *
 * @version     V1.0.0
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2023 Geehy Semiconductor
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
#include "log_printf.h"
#include <stdio.h>

/** @addtogroup Board
  @{
*/

/** @defgroup LOG_Driver
  @{
*/

/** @defgroup LOG_Driver_Functions
  @{
*/

static LOG_FUNC_T logCallback = &vprintf;

/**
 * @brief   Set log callback function
 *
 * @param   callback: Log callback function
 *
 * @retval  None
 */
void LOG_SetCallback(LOG_FUNC_T callback)
{
    logCallback = callback;
}

/**
 * @brief   Print message into log
 *
 * @param   format: Print message format
 *
 * @retval  None
 */
void LOG_Print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    (*logCallback)(format, args);
    va_end(args);
}

/**@} end of group LOG_Driver_Functions */
/**@} end of group LOG_Driver */
/**@} end of group Board */
