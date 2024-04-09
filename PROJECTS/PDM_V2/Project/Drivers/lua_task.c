/*
 * lua_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "lua_task.h"
#include "lua_lib.h"
#include "lauxlib.h"
#include "lua.h"
#include "din_task.h"



static TaskHandle_t  pTaskHandle  	__SECTION(RAM_SECTION_CCMRAM);


TaskHandle_t * xGetLUATaskHandle()
{
	return  &pTaskHandle;
}



#include <hal_timers.h>
#include "lua_task.h"
#include "lprefix.h"
#include "lua.h"
#include "lauxlib.h"
#include "lua_lib.h"
#include "lualib.h"
#include "lapi.h"
#include "drivers_config.h"
#include "adc_task.h"
#include "din_task.h"
//#include "flash_task.h"

static EventGroupHandle_t xPDMstatusEvent 	__SECTION(RAM_SECTION_CCMRAM);
static LUA_STATE_t state 					__SECTION(RAM_SECTION_CCMRAM);
static ENABLE_t eSafeModeIsEnable 			__SECTION(RAM_SECTION_CCMRAM)= IS_DISABLE;
char * pcLuaErrorString 				    __SECTION(RAM_SECTION_CCMRAM) = NULL;
static ENABLE_t eMainLoopIsEnable 			__SECTION(RAM_SECTION_CCMRAM)= IS_DISABLE;
static uint8_t RestartFlag  = 0;
uint32_t ulWorkCicleIn10us					__SECTION(RAM_SECTION_CCMRAM) = 0U;
static uint8_t ucErrorCount 				__SECTION(RAM_SECTION_CCMRAM)= 0U;
int res 									__SECTION(RAM_SECTION_CCMRAM) = 0;
/*
 *
 */
EventGroupHandle_t* osLUAetPDMstatusHandle ( void )
{
  return ( &xPDMstatusEvent );
}
/*
 *
 */
char * pcGetLUAError()
{
	return ( pcLuaErrorString );
}
/*
 *
 */
uint8_t ucLUAgetErrorCount ( void )
{
	return ( ucErrorCount );
}
/*
 *
 */
const char * pcLUAgetErrorString ( void )
{
	return ( pcLuaErrorString );
}
/*
 *
 */
uint32_t ulLUAgetWorkCicle ( void )
{
	return ( ulWorkCicleIn10us );
}
/*
 *
 */
LUA_STATE_t eLUAgetSTATE ( void )
{
  return ( state );
}
/*
 *
 */
static void vSafeModeOutState()
{
	 for (uint8_t i = 0; i<OUT_COUNT; i++ )
	 {
		 vHWOutOFF( i );
		 vHWOutDisable( i );
     }
	 return;
}

/*
 *
 */
void vLUArunPDM()
{
	xEventGroupSetBits(xPDMstatusEvent,RUN_STATE);
	state = LUA_RUN;
	return;
}

/*
 *
 */
void vLUAstopPDM()
{
	xEventGroupClearBits(xPDMstatusEvent,RUN_STATE);
	state = LUA_STOP;
	return;
}
/*
 *
 */
void vLUArestartPDM()
{
	state = LUA_RESTART;
	return;
}

/*
 *
 */
static RESULT_t eIsLuaSkriptValid(const char* pcData, uint32_t size)
{
	uint8_t ucRes = RESULT_FALSE;
	uint8_t ucEND = 0x00;
	for (uint32_t ulIndex = 0;ulIndex < size; ulIndex++)
	{
		if ( pcData[ulIndex] == ucEND )
		{
			if (ucEND == 0x00)
			{
				if ( ( pcData[0] == LUA_SIGNATURE[0] ) && ( pcData[1] == LUA_SIGNATURE[1] ) && ( pcData[2] == LUA_SIGNATURE[2] ) && (pcData[3] == LUA_SIGNATURE[3]) )
				{
					ucEND = 0xFF;
				}
				else
				{
					ucRes = RESULT_TRUE;
					break;
				}
			}
			else
			{
				ucRes = RESULT_TRUE;
				break;
			}
		}
	}
	return ( ucRes );
}

void vRegisterHWLib( lua_State *L1 )
{
	lua_register(L1,"CanTable",iCanSendTable);
	lua_register(L1,"CanSend", iCanSendPDM);
	lua_register(L1,"sendCandRequest",iCanSendRequest);
	lua_register(L1,"setCanFilter", iCanSetResiveFilter);
	lua_register(L1,"CheckCanId", iCanCheckData);
	lua_register(L1,"GetCanMessage",iCanGetMessage);
	lua_register(L1,"GetCanToTable",iCanGetResivedData);
	lua_register(L1,"CheckAnswer", iCanCheckData);
	lua_register(L1,"GetRequest",iCanGetMessage);
	lua_register(L1,"GetRequestToTable",iCanGetResivedData);
	lua_register(L1,"ConfigCan",iCanSetConfig);
	lua_register(L1,"setDINConfig",iDinConfig);
	lua_register(L1,"RPMConfig",iSetRPMConfig);
	lua_register(L1,"setAINCalTable",isetAINCal);
	lua_register(L1,"setOutConfig", iOutConfig);
	lua_register(L1,"OutResetConfig", iOutResetConfig);
	lua_register(L1,"OutSetPWM", iOutSetPWM);
	lua_register(L1,"setPWMGroupeFreq",isetPWMFreq);
	lua_register(L1,"SYSTEM_RESTASRT",iSysrestart);
	lua_register(L1,"GetEEPROMReg",iGetEEPROM);
	lua_register(L1,"SetEEPROMReg",iSetEEPROM);
	lua_register(L1,"SetTimeDate",iSetTime);
	lua_register(L1,"GetTimeDate",iGetTime);
	lua_register(L1,"AddReccord",iSetRecord);
	lua_register(L1,"ConfigStorage",iSetStorageFormat);
}

void vLuaTask1(void *argument)
{
	uint32_t OutStatus1   = 0;
    uint32_t OutStatus2   = 0;
	lua_State *L = NULL;
	lua_State *L1 = NULL;
	vHW_L_LIB_FreeRunInit(TIMER11,100000);
	vHAL_TiemrEneblae( TIMER11);
	while(1)
	{
		 vTaskDelay( 1 );
		 switch (state)
		 {
		       case LUA_INIT:
		    	   RestartFlag = 0;
		    	   vSetRestartFlag(&RestartFlag);
		    	 //  vOutInit();
		       //    vAINInit();
		        //   vDINRestart();
		    	   eMainLoopIsEnable  = IS_DISABLE;
			   	   eSafeModeIsEnable  = IS_DISABLE;
			   	   L  = luaL_newstate();
			   	   L1 = lua_newthread(L);
			   	   luaL_openlibs(L1); // open standard libraries
			   	   vRegisterHWLib(L1 );
			   	   vLUArunPDM();
			   	   if ( eIsLuaSkriptValid(uFLASHgetScript(), uFLASHgetLength()+1) == RESULT_TRUE )
			   	   {
			   	    	res =(luaL_loadbuffer(L1, uFLASHgetScript(), uFLASHgetLength() , uFLASHgetScript())  || lua_pcall(L1, 0, LUA_MULTRET, 0));
			   	   }
			   	   else
			   	   {
			   	   		 state = LUA_ERROR;
			   	   }
			   	   break;
			   	 case LUA_RUN:
			   	   lua_getglobal(L1, "main");
			   	   ulWorkCicleIn10us  = uGetFreeRuningTimer(TIMER11 );
			   	   lua_pushinteger(L1, ulWorkCicleIn10us );
			   	   vGetDoutStatus(&OutStatus1 , &OutStatus2 );
			   	   lua_pushinteger( L1, OutStatus1 );
			   	   lua_pushinteger( L1, OutStatus2 );
			   	   lua_pushinteger( L1, uiGetDinMask() );
		           for ( uint8_t i = 0U; i < OUT_COUNT ; i++ )
			   	   {
		        	   lua_pushnumber( L1, fOutGetCurrent( i ) );
			   	   }
		           lua_pushnumber( L1, uGetRPM1( ) );
		           lua_pushnumber( L1, uGetRPM2( ) );
		           lua_pushnumber( L1, fAinGetState(0));
		           lua_pushnumber( L1, fAinGetState(1));
		           lua_pushnumber( L1, fAinGetState(2));
		           lua_pushnumber( L1, fBatteryGet() );
		           lua_pushnumber( L1, 0x00);// fAngleGet (ANGLE_TYPE_ROLL) );
		           lua_pushnumber( L1, 0x00);//fAngleGet (ANGLE_TYPE_PITCH) );
		           lua_pushnumber( L1, 0x00);//fAngleGet (ANGLE_TYPE_YAW) );
		           lua_pushnumber( L1, fTemperatureGet () );
		           int temp;
		           switch ( lua_resume( L1, L, (1+1+2+OUT_COUNT+2+4+3+1), &temp) )
			   	   {
			   	     case  LUA_OK:
			   	   	   if (eMainLoopIsEnable == IS_DISABLE)
			   	   	   {
			   	   		   eMainLoopIsEnable = IS_ENABLE;
			   	   	   }
			   	   	 case LUA_YIELD:
			   	   		 for ( uint8_t i = 0; i < OUT_COUNT; i++)
			   	   		 {
			   	   			 vOutSetState( i, (uint8_t) lua_toboolean( L1,-(i+1)) );
			   	   		 }
			   	   		 break;
			   	   	 default:
			   	   	   pcLuaErrorString =  (char *) lua_tostring( L1, LAST_ARGUMENT );
			   	   	   ucErrorCount++;
			   	   	   state  = LUA_ERROR;
			   	   	   break;
			   	   }
			   	   lua_pop( L1, temp);
			   	   if (RestartFlag != 0)
			   	   {
		  	   		  vSafeModeOutState();
			   		  state = LUA_RESTART;
			   	   }
			   	   break;
			   	 case LUA_ERROR:
			   	 case LUA_STOP:
			   		if (eSafeModeIsEnable == IS_DISABLE)
			   		{
			   			vSafeModeOutState();
			   			eSafeModeIsEnable = IS_ENABLE;
			   		}
			   	   break;
			   	 case LUA_RESTART:
			   	   lua_close(L);
			   	   state = LUA_INIT;
			   	   break;
			   	 default:
			   	   break;
			  }

	}
}


void vLuaTask(void *argument)
{

	while(1)
	{
		vTaskDelay(100);
	}
}
