/*!
 * @file        usbd_hid.h
 *
 * @brief       usb device hid class handler header file
 *
 * @version     V1.0.0
 *
 * @date        2023-03-01
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

/* Define to prevent recursive inclusion */
#ifndef _USBD_HID_H_
#define _USBD_HID_H_

/* Includes */
#include "usbd_core.h"

/** @addtogroup APM32_USB_Library
  @{
  */

/** @addtogroup USBD_HID_Class
  @{
  */

/** @defgroup USBD_HID_Macros Macros
  @{
*/

#define USBD_HID_MOUSE_REPORT_DESC_SIZE         74
#define USBD_HID_KEYBOARD_REPORT_DESC_SIZE      63
#define USBD_HID_DESC_SIZE                      9
#define USBD_HID_FS_INTERVAL                    10
#define USBD_HID_HS_INTERVAL                    7
#define USBD_HID_IN_EP_ADDR                     0x81
#define USBD_HID_IN_EP_SIZE                     0x04
#define USBD_HID_FS_MP_SIZE                     0x40

#define USBD_CLASS_SET_IDLE                     0x0A
#define USBD_CLASS_GET_IDLE                     0x02

#define USBD_CLASS_SET_REPORT                   0x09
#define USBD_CLASS_GET_REPORT                   0x01

#define USBD_CLASS_SET_PROTOCOL                 0x0B
#define USBD_CLASS_GET_PROTOCOL                 0x03

/**@} end of group USBD_HID_Macros*/

/** @defgroup USBD_HID_Enumerates Enumerates
  @{
  */

/**
 * @brief    HID state type
 */
typedef enum
{
    USBD_HID_IDLE,
    USBD_HID_BUSY,
} USBD_HID_STATE_T;

/**
 * @brief    HID keyboard value
 */
typedef enum
{
    KEYBOARD_NONE,
    KEYBOARD_ERROR_ROLL_OVER,
    KEYBOARD_POST_FAIL,
    KEYBOARD_ERROR_UNDEFINED,
    KEYBOARD_A,
    KEYBOARD_B,
    KEYBOARD_C,
    KEYBOARD_D,
    KEYBOARD_E,
    KEYBOARD_F,
    KEYBOARD_G,
    KEYBOARD_H,
    KEYBOARD_I,
    KEYBOARD_J,
    KEYBOARD_K,
    KEYBOARD_L,
    KEYBOARD_M,
    KEYBOARD_N,
    KEYBOARD_O,
    KEYBOARD_P,
    KEYBOARD_Q,
    KEYBOARD_R,
    KEYBOARD_S,
    KEYBOARD_T,
    KEYBOARD_U,
    KEYBOARD_V,
    KEYBOARD_W,
    KEYBOARD_X,
    KEYBOARD_Y,
    KEYBOARD_Z,
    KEYBOARD_1_EXCLAMATION,
    KEYBOARD_2_AT,
    KEYBOARD_3_NUMBER_SIGN,
    KEYBOARD_4_DOLLAR,
    KEYBOARD_5_PERCENT,
    KEYBOARD_6_CARET,
    KEYBOARD_7_AMPERSAND,
    KEYBOARD_8_ASTERISK,
    KEYBOARD_9_OPARENTHESIS,
    KEYBOARD_10_CPARENTHESIS,
    KEYBOARD_ENTER,
    KEYBOARD_ESCAPE,
    KEYBOARD_BACKSPACE,
    KEYBOARD_TAB,
    KEYBOARD_SPACEBAR,
    KEYBOARD_MINUS_UNDERSCORE,
    KEYBOARD_EQUAL_PLUS,
    KEYBOARD_OBRACKET_AND_OBRACE,
    KEYBOARD_CBRACKET_AND_CBRACE,
    KEYBOARD_BACKSLASH_VERTICAL_BAR,
    KEYBOARD_NONUS_NUMBER_SIGN_TILDE,
    KEYBOARD_SEMICOLON_COLON,
    KEYBOARD_SINGLE_AND_DOUBLE_QUOTE,
    KEYBOARD_GRAVE_ACCENT_AND_TILDE,
    KEYBOARD_COMMA_AND_LESS,
    KEYBOARD_DOT_GREATER,
    KEYBOARD_SLASH_QUESTION,
    KEYBOARD_CAPS_LOCK,
    KEYBOARD_F1,
    KEYBOARD_F2,
    KEYBOARD_F3,
    KEYBOARD_F4,
    KEYBOARD_F5,
    KEYBOARD_F6,
    KEYBOARD_F7,
    KEYBOARD_F8,
    KEYBOARD_F9,
    KEYBOARD_F10,
    KEYBOARD_F11,
    KEYBOARD_F12,
    KEYBOARD_PRINTSCREEN,
    KEYBOARD_SCROLL_LOCK,
    KEYBOARD_PAUSE,
    KEYBOARD_INSERT,
    KEYBOARD_HOME,
    KEYBOARD_PAGEUP,
    KEYBOARD_DELETE,
    KEYBOARD_END1,
    KEYBOARD_PAGEDOWN,
    KEYBOARD_RIGHTARROW,
    KEYBOARD_LEFTARROW,
    KEYBOARD_DOWNARROW,
    KEYBOARD_UPARROW,
    KEYBOARD_KEYBOARDPAD_NUM_LOCK_AND_CLEAR,
    KEYBOARD_KEYBOARDPAD_SLASH,
    KEYBOARD_KEYBOARDPAD_ASTERIKS,
    KEYBOARD_KEYBOARDPAD_MINUS,
    KEYBOARD_KEYBOARDPAD_PLUS,
    KEYBOARD_KEYBOARDPAD_ENTER,
    KEYBOARD_KEYBOARDPAD_1_END,
    KEYBOARD_KEYBOARDPAD_2_DOWN_ARROW,
    KEYBOARD_KEYBOARDPAD_3_PAGEDN,
    KEYBOARD_KEYBOARDPAD_4_LEFT_ARROW,
    KEYBOARD_KEYBOARDPAD_5,
    KEYBOARD_KEYBOARDPAD_6_RIGHT_ARROW,
    KEYBOARD_KEYBOARDPAD_7_HOME,
    KEYBOARD_KEYBOARDPAD_8_UP_ARROW,
    KEYBOARD_KEYBOARDPAD_9_PAGEUP,
    KEYBOARD_KEYBOARDPAD_0_INSERT,
    KEYBOARD_KEYBOARDPAD_DECIMAL_SEPARATOR_DELETE,
    KEYBOARD_NONUS_BACK_SLASH_VERTICAL_BAR,
    KEYBOARD_APPLICATION,
    KEYBOARD_POWER,
    KEYBOARD_KEYBOARDPAD_EQUAL,
    KEYBOARD_F13,
    KEYBOARD_F14,
    KEYBOARD_F15,
    KEYBOARD_F16,
    KEYBOARD_F17,
    KEYBOARD_F18,
    KEYBOARD_F19,
    KEYBOARD_F20,
    KEYBOARD_F21,
    KEYBOARD_F22,
    KEYBOARD_F23,
    KEYBOARD_F24,
    KEYBOARD_EXECUTE,
    KEYBOARD_HELP,
    KEYBOARD_MENU,
    KEYBOARD_SELECT,
    KEYBOARD_STOP,
    KEYBOARD_AGAIN,
    KEYBOARD_UNDO,
    KEYBOARD_CUT,
    KEYBOARD_COPY,
    KEYBOARD_PASTE,
    KEYBOARD_FIND,
    KEYBOARD_MUTE,
    KEYBOARD_VOLUME_UP,
    KEYBOARD_VOLUME_DOWN,
    KEYBOARD_LOCKING_CAPS_LOCK,
    KEYBOARD_LOCKING_NUM_LOCK,
    KEYBOARD_LOCKING_SCROLL_LOCK,
    KEYBOARD_KEYBOARDPAD_COMMA,
    KEYBOARD_KEYBOARDPAD_EQUAL_SIGN,
    KEYBOARD_INTERNATIONAL1,
    KEYBOARD_INTERNATIONAL2,
    KEYBOARD_INTERNATIONAL3,
    KEYBOARD_INTERNATIONAL4,
    KEYBOARD_INTERNATIONAL5,
    KEYBOARD_INTERNATIONAL6,
    KEYBOARD_INTERNATIONAL7,
    KEYBOARD_INTERNATIONAL8,
    KEYBOARD_INTERNATIONAL9,
    KEYBOARD_LANG1,
    KEYBOARD_LANG2,
    KEYBOARD_LANG3,
    KEYBOARD_LANG4,
    KEYBOARD_LANG5,
    KEYBOARD_LANG6,
    KEYBOARD_LANG7,
    KEYBOARD_LANG8,
    KEYBOARD_LANG9,
    KEYBOARD_ALTERNATE_ERASE,
    KEYBOARD_SYSREQ,
    KEYBOARD_CANCEL,
    KEYBOARD_CLEAR,
    KEYBOARD_PRIOR,
    KEYBOARD_RETURN,
    KEYBOARD_SEPARATOR,
    KEYBOARD_OUT,
    KEYBOARD_OPER,
    KEYBOARD_CLEAR_AGAIN,
    KEYBOARD_CRSEL,
    KEYBOARD_EXSEL,
    KEYBOARD_RESERVED1,
    KEYBOARD_RESERVED2,
    KEYBOARD_RESERVED3,
    KEYBOARD_RESERVED4,
    KEYBOARD_RESERVED5,
    KEYBOARD_RESERVED6,
    KEYBOARD_RESERVED7,
    KEYBOARD_RESERVED8,
    KEYBOARD_RESERVED9,
    KEYBOARD_RESERVED10,
    KEYBOARD_RESERVED11,
    KEYBOARD_KEYBOARDPAD_00,
    KEYBOARD_KEYBOARDPAD_000,
    KEYBOARD_THOUSANDS_SEPARATOR,
    KEYBOARD_DECIMAL_SEPARATOR,
    KEYBOARD_CURRENCY_UNIT,
    KEYBOARD_CURRENCY_SUB_UNIT,
    KEYBOARD_KEYBOARDPAD_OPARENTHESIS,
    KEYBOARD_KEYBOARDPAD_CPARENTHESIS,
    KEYBOARD_KEYBOARDPAD_OBRACE,
    KEYBOARD_KEYBOARDPAD_CBRACE,
    KEYBOARD_KEYBOARDPAD_TAB,
    KEYBOARD_KEYBOARDPAD_BACKSPACE,
    KEYBOARD_KEYBOARDPAD_A,
    KEYBOARD_KEYBOARDPAD_B,
    KEYBOARD_KEYBOARDPAD_C,
    KEYBOARD_KEYBOARDPAD_D,
    KEYBOARD_KEYBOARDPAD_E,
    KEYBOARD_KEYBOARDPAD_F,
    KEYBOARD_KEYBOARDPAD_XOR,
    KEYBOARD_KEYBOARDPAD_CARET,
    KEYBOARD_KEYBOARDPAD_PERCENT,
    KEYBOARD_KEYBOARDPAD_LESS,
    KEYBOARD_KEYBOARDPAD_GREATER,
    KEYBOARD_KEYBOARDPAD_AMPERSAND,
    KEYBOARD_KEYBOARDPAD_LOGICAL_AND,
    KEYBOARD_KEYBOARDPAD_VERTICAL_BAR,
    KEYBOARD_KEYBOARDPAD_LOGIACL_OR,
    KEYBOARD_KEYBOARDPAD_COLON,
    KEYBOARD_KEYBOARDPAD_NUMBER_SIGN,
    KEYBOARD_KEYBOARDPAD_SPACE,
    KEYBOARD_KEYBOARDPAD_AT,
    KEYBOARD_KEYBOARDPAD_EXCLAMATION_MARK,
    KEYBOARD_KEYBOARDPAD_MEMORY_STORE,
    KEYBOARD_KEYBOARDPAD_MEMORY_RECALL,
    KEYBOARD_KEYBOARDPAD_MEMORY_CLEAR,
    KEYBOARD_KEYBOARDPAD_MEMORY_ADD,
    KEYBOARD_KEYBOARDPAD_MEMORY_SUBTRACT,
    KEYBOARD_KEYBOARDPAD_MEMORY_MULTIPLY,
    KEYBOARD_KEYBOARDPAD_MEMORY_DIVIDE,
    KEYBOARD_KEYBOARDPAD_PLUSMINUS,
    KEYBOARD_KEYBOARDPAD_CLEAR,
    KEYBOARD_KEYBOARDPAD_CLEAR_ENTRY,
    KEYBOARD_KEYBOARDPAD_BINARY,
    KEYBOARD_KEYBOARDPAD_OCTAL,
    KEYBOARD_KEYBOARDPAD_DECIMAL,
    KEYBOARD_KEYBOARDPAD_HEXADECIMAL,
    KEYBOARD_RESERVED12,
    KEYBOARD_RESERVED13,
    KEYBOARD_LEFTCONTROL,
    KEYBOARD_LEFTSHIFT,
    KEYBOARD_LEFTALT,
    KEYBOARD_LEFT_GUI,
    KEYBOARD_RIGHTCONTROL,
    KEYBOARD_RIGHTSHIFT,
    KEYBOARD_RIGHTALT,
    KEYBOARD_RIGHT_GUI,
} USBH_HID_KEYBOARD_VALUE_T;

/**@} end of group USBD_HID_Enumerates*/

/** @defgroup USBD_HID_Structures Structures
  @{
  */

/**
 * @brief    HID information management
 */
typedef struct
{
    uint8_t             state;
    uint8_t             epInAddr;
    uint8_t             altSettingStatus;
    uint8_t             idleStatus;
    uint8_t             protocol;
} USBD_HID_INFO_T;

extern USBD_CLASS_T USBD_HID_CLASS;

/**@} end of group USBD_HID_Structures*/

/** @defgroup USBD_HID_Functions Functions
  @{
  */

uint8_t USBD_HID_ReadInterval(USBD_INFO_T* usbInfo);
USBD_STA_T USBD_HID_TxReport(USBD_INFO_T* usbInfo, uint8_t* report, uint16_t length);

/**@} end of group USBD_HID_Functions */
/**@} end of group USBD_HID_Class */
/**@} end of group APM32_USB_Library */

#endif
