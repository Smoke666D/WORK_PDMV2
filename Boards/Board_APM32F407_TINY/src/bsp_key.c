/*!
 * @file        bsp_key.c
 *
 * @brief       Key board support package body
 *
 * @version     V1.0.1
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
 #include "bsp_key.h"

/** @addtogroup Board
  @{
*/

/** @addtogroup Board_APM32F407_TINY
  @{
*/

/** @defgroup APM32F407_TINY_Fuctions
  @{
  */

 /*!
 * @brief    Scan KEY1 and KEY2
 *
 * @param    None
 *
 * @retval   key value
 *
 */
uint8_t Key_Scan(uint8_t mode)
{
    /* The flag of button is release */
    static uint8_t s_key_release = 1;

    if(mode == KEY_MODE_CONTINUOUS)
    {
        s_key_release = 1;
    }

    if(s_key_release && (KEY1 == 0 || KEY2 == 0))
    {
        Delay_ms(10);

        s_key_release = 0;

        if(KEY1 == 0)
        {
            return KEY1_PRESSED;
        }
        else if(KEY2 == 0)
        {
            return KEY2_PRESSED;
        }
    }
    else if(KEY1 == 1 && KEY2 == 1)
    {
        s_key_release = 1;
    }

    return KEY_ALL_REALEASED;
}

/**@} end of group APM32F407_TINY_Functions */
/**@} end of group Board_APM32F407_TINY */
/**@} end of group Board */
