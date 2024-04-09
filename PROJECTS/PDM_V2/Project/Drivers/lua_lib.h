/*
 * lua_lib.h
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#ifndef DRIVERS_LUA_LIB_H_
#define DRIVERS_LUA_LIB_H_

#include "lua.h"


#define CAN_FRAME_SIZE  				8U
#define CAN_ID_POS					    1U
#define FRAME_SIZE_POS					2U
#define CAN_TABLE_POS					3U
#define CANREQSEND_ARGUMENT_COUNT 3
#define SEND_REQUEST_ARGUMENT_COUNT 3
#define CAN_SEND_TABLE_ARGUMENT_COUNT 	3U

#define FIRST_ARGUMENT   1U
#define SECOND_ARGUMENT  2U
#define THIRD_ARGUMENT   3U
#define FOURTH_ARGUMENT  4U
#define FIVE_ARGUMENT	 5U
#define SIX_ARGUMENT	 6U
#define SEVEN_ARGUMENT   7U
#define LAST_ARGUMENT	-1

#define ONE_ARGUMENT	1U
#define TWO_ARGUMENTS	2U
#define THREE_ARGUMENTS  3U
#define FOUR_ARGUMENTS  4U

#define NO_RESULT  0U
#define ONE_RESULT 1U
#define TWO_RESULT 2U
#define SEVEN_RESULT   7U
#define SUCSESS    1U
//#define ERROR	   0U

int iSetTime( lua_State *L );
int iGetTime( lua_State *L );

void vSetRestartFlag( uint8_t * flag);
int iSysrestart(lua_State *L);

int iCanSendTable( lua_State *L );
int iCanSendPDM( lua_State *L );
int iCanSendRequest( lua_State *L );
int iCanSetResiveFilter(lua_State *L );
int iCanCheckData(lua_State *L );
int iCanGetResivedData(lua_State *L );
int iCanGetMessage(lua_State *L );
int iCanSetConfig(lua_State *L);

int iDinConfig(lua_State *L );
int iSetRPMConfig(lua_State *L );

int isetAINCal (lua_State *L);

int  iOutConfig( lua_State *L );
int  iOutResetConfig( lua_State *L );
int  iOutSetPWM( lua_State *L );
int  isetPWMFreq(lua_State *L);

int iGetEEPROM( lua_State *L );
int iSetEEPROM( lua_State *L );
int iSetStorageFormat( lua_State *L );
int iSetRecord( lua_State *L );

#endif /* DRIVERS_LUA_LIB_H_ */
