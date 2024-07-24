/*
 * version.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_VERSION_H_
#define DRIVERS_VERSION_H_


/*----------------- BOOTLOADER VERSION -----------------*/
#define BOOTLOADER_VERSION_ADR  0x8007FFCU
/*------------------ HARDWARE VERSION ------------------*/
#define HARDWARE_VERSION_MAJOR  3
#define HARDWARE_VERSION_MINOR  3
#define HARDWARE_VERSION_PATCH  0
#if ( HARDWARE_VERSION_MAJOR > 255U )
  #error( "Major hardware version too big" )
#endif
#if ( HARDWARE_VERSION_MINOR > 255U )
  #error( "Minor hardware version too big" )
#endif
#if ( HARDWARE_VERSION_PATCH > 255U )
  #error( "Patch hardware version too big" )
#endif
/*------------------ FIRMWARE VERSION ------------------*/
#define FIRMWARE_VERSION_MAJOR  3
#define FIRMWARE_VERSION_MINOR  1
#define FIRMWARE_VERSION_PATCH  0
#if ( FIRMWARE_VERSION_MAJOR > 255U )
  #error( "Major firmware version too big" )
#endif
#if ( FIRMWARE_VERSION_MINOR > 255U )
  #error( "Minor firmware version too big" )
#endif
#if ( FIRMWARE_VERSION_PATCH > 255U )
  #error( "Patch firmware version too big" )
#endif
/*------------------------------------------------------*/
typedef struct
{
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
} VERSION;
/*------------------------------------------------------*/



#endif /* DRIVERS_VERSION_H_ */
