/*
 * lua_task.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */

#include "lua_task.h"
#include "lprefix.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lapi.h"
#include "din_task.h"
#include "flash_task.h"
#include "hw_lib_datastorage.h"





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
#include "string.h"

static uint8_t ucErrorCount 				__SECTION(RAM_SECTION_CCMRAM)= 0U;
static EventGroupHandle_t xPDMstatusEvent 	__SECTION(RAM_SECTION_CCMRAM);
static LUA_STATE_t lua_state 				__SECTION(RAM_SECTION_CCMRAM);
static char * pcLuaErrorString 				__SECTION(RAM_SECTION_CCMRAM);
static uint32_t ulWorkCicleIn10us			__SECTION(RAM_SECTION_CCMRAM);
int res 									__SECTION(RAM_SECTION_CCMRAM);
static TaskHandle_t  pLuaTaskHandle  	    __SECTION(RAM_SECTION_CCMRAM);
static ENABLE_t eSafeModeIsEnable 			__SECTION(RAM_SECTION_CCMRAM)= IS_DISABLE;
static uint8_t RestartFlag  = 0;



static void vCCMRAVarInir()
{
	lua_state 				= LUA_INIT;
    ucErrorCount 	  	= 0U;
    ulWorkCicleIn10us 	= 0U;
    eSafeModeIsEnable 	= IS_DISABLE;
    pcLuaErrorString  	= NULL;
    res 				= 0;
	return;
}
TaskHandle_t * xGetLUATaskHandle()
{
	return  &pLuaTaskHandle;
}
/*
 *
 */
EventGroupHandle_t* osLUAetPDMstatusHandle ( void )
{
  return ( &xPDMstatusEvent );
}


uint8_t ucLUAgetErrorCount ( void )
{
	return ( ucErrorCount );
}
/*
 *
 */
const char * pcGetLUAError ( void )
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
  return ( lua_state );
}
/*
 *
 */

void vLUArunPDM()
{
	xEventGroupSetBits(xPDMstatusEvent,RUN_STATE);
	lua_state = LUA_RUN;
	return;
}

/*void vSetInitState()
{
	xEventGroupClearBits(xPDMstatusEvent,PDM_RUN_STATE | PDM_STOP_STATE );
	lua_state = LUA_INIT;
}*/

void vLUAstopPDM()
{
	xEventGroupClearBits(xPDMstatusEvent,RUN_STATE);
	lua_state = LUA_STOP;
	return;
}
/*
 *
 */
void vLUArestartPDM()
{
	lua_state = LUA_RESTART;
	return;
}
/*void vSetRunState()
{

	lua_state = LUA_RUN;
	xEventGroupSetBits(xPDMstatusEvent, PDM_RUN_STATE );

}
void vStopPDMState( LUA_STATE_t state)
{
	xEventGroupClearBits(xPDMstatusEvent, PDM_RUN_STATE );
	lua_state = state;


}*/

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
 * Функция управленя признаком RandomReset
 */
static int iSetRandomRestart(lua_State *L)
{
	if (lua_gettop(L) ==  THREE_ARGUMENTS)
	{
		uint8_t ucNumber =(uint8_t) lua_tointeger( L, FIRST_ARGUMENT); //First argument it's channel number
		uint8_t ucState =(uint8_t) lua_tointeger( L, SECOND_ARGUMENT);
		uint8_t ucCoolDownTime =(uint8_t) lua_tointeger( L,  THIRD_ARGUMENT);
		vSetRendomResetState( ucNumber -1u, ucState, ucCoolDownTime );
	}
	return ( NO_RESULT );

}

/*
 *
 */
/*(void vLUAstopPDM()
{
	EventBits_t state = xEventGroupWaitBits(xPDMstatusEvent, PDM_STOP_STATE | PDM_RUN_STATE, pdFALSE,pdFALSE, portMAX_DELAY );
	if ( state & PDM_RUN_STATE)
	{
		xTaskNotifyGiveIndexed(pLuaTaskHandle, 1);
		xEventGroupWaitBits(xPDMstatusEvent, PDM_STOP_STATE, pdFALSE,pdTRUE, portMAX_DELAY );
	}
	return;
}*/
/*
 *
 */
/*void vLUArestartPDM()
{
	EventBits_t CurState  = xEventGroupWaitBits(xPDMstatusEvent, PDM_STOP_STATE | PDM_RUN_STATE, pdFALSE,pdFALSE, portMAX_DELAY );
	if ( ( CurState &  PDM_STOP_STATE) )
	{
		xTaskNotifyGiveIndexed(pLuaTaskHandle, 1);
		xEventGroupSetBits(xPDMstatusEvent,PDM_RUN_STATE );
	}
	if (  CurState & ( PDM_RUN_STATE) )
	{
		xTaskNotifyGiveIndexed(pLuaTaskHandle, 1);
		xEventGroupWaitBits(xPDMstatusEvent, PDM_STOP_STATE, pdFALSE,pdTRUE, portMAX_DELAY );
		xTaskNotifyGiveIndexed(pLuaTaskHandle, 1);
		xEventGroupSetBits(xPDMstatusEvent,PDM_RUN_STATE );
	}
	//SoftwareReset();
	return;
}

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
	lua_register(L1,"RandomRestart",iSetRandomRestart);
	lua_register(L1,"GetEEPROMReg",iGetEEPROM);
	lua_register(L1,"SetEEPROMReg",iSetEEPROM);
	lua_register(L1,"SetTimeDate",iSetTime);
	lua_register(L1,"GetTimeDate",iGetTime);
	lua_register(L1,"AddReccord",iSetRecord);
	lua_register(L1,"ConfigStorage",iSetStorageFormat);
}

void vLuaTask(void *argument)
{
	uint32_t ulNVRestart;
	uint32_t ulNotifiedValue;
	uint32_t OutStatus1   = 0;
    uint32_t OutStatus2   = 0;
	lua_State *L = NULL;
	lua_State *L1 = NULL;
	vHW_L_LIB_FreeRunInit(TIMER11,100000);
	HAL_TiemrEneblae( TIMER11);
	vCCMRAVarInir();
	while(1)
	{
		 vTaskDelay( 1 );
		 switch (lua_state)
		 {
		       case LUA_INIT:
		    	 RestartFlag = 0;
		         vAINInit();
		         vOutInit();
		         vDINInit();

		       	 eSafeModeIsEnable  = IS_DISABLE;
		       //  vTestEEPROM();
			   	 L  = luaL_newstate();
			   	 L1 = lua_newthread(L);
			   	 luaL_openlibs(L1); // open standard libraries
			   	 vRegisterHWLib(L1 );
			   	 vLUArunPDM();
			   	 if ( eIsLuaSkriptValid(uFLASHgetScript(), uFLASHgetLength()+1) == RESULT_TRUE )
			     {
			   		res = (luaL_loadbuffer(L1, uFLASHgetScript(), uFLASHgetLength() , uFLASHgetScript())  || lua_pcall(L1, 0, LUA_MULTRET, 0));
			   			//ulTaskNotifyValueClearIndexed(NULL, 0, 0xFFFF);
			   			//DinNotifyTaskToInit();
			   			//AinNotifyTaskToInit();
			   		//	lua_state  = LUA_WAIT_READY;
			   		// }
			     }
			   	 else
			      {
			   		lua_state = LUA_ERROR;
			      }
			   	  break;
		          /*case LUA_WAIT_READY:
		        	xTaskNotifyWait(0, 0, &ulNotifiedValue,portMAX_DELAY);
		        	if ( ulNotifiedValue == 2)
		        	{
		        		ulTaskNotifyValueClearIndexed(NULL, 0, 0xFFFF);
		        		vSetRunState();
		        	}
		        	break;*/
			   	 case LUA_RUN:
			   	  /* if ( xTaskNotifyWaitIndexed(1,0x00,0xFF, &ulNVRestart ,0) == pdTRUE)
			   	   {
			   		   vStopPDMState(LUA_STOP);
			   		   break;
			   	   }*/
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
		           lua_pushnumber( L1, fTemperatureGet (1) );
		           int temp;
		           switch ( lua_resume( L1, L, (1+1+2+OUT_COUNT+2+4+3+1), &temp) )
			   	   {
			   	     case  LUA_OK:
			   	   	 case LUA_YIELD:
			   	   		 for ( uint8_t i = 0; i < OUT_COUNT; i++)
			   	   		 {
			   	   			 vOutSetState( i, (uint8_t) lua_toboolean( L1,-(i+1)) );
			   	   		 }
			   	   		 break;
			   	   	 default:
			   	   	   pcLuaErrorString =  (char *) lua_tostring( L1, LAST_ARGUMENT );
			   	       ucErrorCount++;
			   	       lua_state  = LUA_ERROR;;
			   	   	   break;
			   	   }
			   	   lua_pop ( L1, temp);
			   	   if (RestartFlag != 0)
			   	   {
			   	  	   	vSafeModeOutState();
			   	  	   	lua_state = LUA_RESTART;
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
			   	 default:
			   		 lua_close(L);
			   	     lua_state = LUA_INIT;

			   	   break;
			  }

	}
}



