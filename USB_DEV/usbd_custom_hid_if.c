/*!
 * @file        usbd_custom_hid_if.c
 *
 * @brief       usb device Custom HID class handler
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
#include "usbd_custom_hid_if.h"
#include "usb_device_user.h"
#include "usb_task.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup OTGD_Custom_HID
  @{
  */

/** @defgroup OTGD_Custom_HID_Variables Variables
  @{
  */

/**
 * @brief   CUSTOM HID report descriptor
 */
uint8_t USBD_CUSTOM_HIDReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] =
{
  /* USER CODE BEGIN 0 */
  0x06, 0x00, 0xff,              //   USAGE_PAGE (Generic Desktop)
  0x09, 0x01,                    //   USAGE (Vendor Usage 1)
  // System Parameters
  0xa1, 0x01,                    //   COLLECTION (Application)
  0x85, 0x01,                    //   REPORT_ID (1)
  0x09, 0x01,                    //   USAGE (Vendor Usage 1)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x95, REPORT_COUNT,            //   REPORT_COUNT (16)
  0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
  0x85, 0x01,                    //   REPORT_ID (1)
  0x09, 0x01,                    //   USAGE (Vendor Usage 1)
  0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

  0x85, 0x02,                    //   REPORT_ID (2)
  0x09, 0x02,                    //   USAGE (Vendor Usage 2)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x95, REPORT_COUNT,            //   REPORT_COUNT (16)
  0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};
/**@} end of group OTGD_Custom_HID_Variables*/

/** @defgroup OTGD_Custom_HID_Structures Structures
  @{
  */

/* USB CUSTOM HID interface handler */
USBD_CUSTOM_HID_INTERFACE_T USBD_CUSTOM_HID_INTERFACE_FS =
{
    "CUSTOM HID Interface FS",
    USBD_CUSTOM_HIDReportDesc,
    USBD_FS_CUSTOM_HID_ItfInit,
    USBD_FS_CUSTOM_HID_ItfDeInit,
    USBD_FS_CUSTOM_HID_ItfSendReport,
    USBD_FS_CUSTOM_HID_ItfReceive
};

/**@} end of group OTGD_Custom_HID_Structures*/

/** @defgroup OTGD_Custom_HID_Functions Functions
  @{
  */

/*!
 * @brief       USB device initializes CUSTOM HID media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CUSTOM_HID_ItfInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;
    vUSBplugHandler();
    return usbStatus;
}

/*!
 * @brief       USB device deinitializes CUSTOM HID media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CUSTOM_HID_ItfDeInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;
    vUSBunplugHandler();
    return usbStatus;
}

/*!
 * @brief       USB device CUSTOM HID interface send handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CUSTOM_HID_ItfSendReport(uint8_t *buffer, uint8_t length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    usbStatus = USBD_CUSTOM_HID_TxReport(&gUsbDeviceFS, buffer, length);

    return usbStatus;
}

/*!
 * @brief       USB device CUSTOM HID interface receive handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CUSTOM_HID_ItfReceive(uint8_t *buffer, uint8_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_CUSTOM_HID_RxPacket(&gUsbDeviceFS);
    
    USBD_CUSTOM_HID_INFO_T * usbDevHID = (USBD_CUSTOM_HID_INFO_T *)gUsbDeviceFS.devClass[gUsbDeviceFS.classID]->classData;
    vUSBreceiveHandler (usbDevHID->report, 0);

    return usbStatus;
}

/**@} end of group OTGD_Custom_HID_Functions */
/**@} end of group OTGD_Custom_HID */
/**@} end of group Examples */
