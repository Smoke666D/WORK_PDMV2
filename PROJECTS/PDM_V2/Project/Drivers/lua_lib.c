/*
 * lua_lib.c
 *
 *  Created on: 9 апр. 2024 г.
 *      Author: i.dymov
 */


#include "lua_lib.h"
#include "lua_task.h"
#include "can_task.h"
#include "drivers_config.h"
#include "din_task.h"
#include "hw_lib_din.h"
#include "adc_task.h"
#include "hw_lib_can.h"
#include "hw_lib_datastorage.h"
#include "apm32f4xx_rtc.h"
#include "lprefix.h"
#include "lua.h"
#include "lauxlib.h"



/*
 *
 *  Блок работы с хранилщем данных
 */

 int iSetEEPROM( lua_State *L )
{
	uint32_t  res = ERROR, temp_bool,temp_int;
	float temp_float;
	RTC_TimeConfig_T time_buffer;
	RTC_DateConfig_T date_buffer;
	PDM_DATA_TIME temp_time;
	uint16_t adr  = lua_tointeger( L , FIRST_ARGUMENT );
	switch (lua_gettop(L))
	{
		case TWO_ARGUMENTS:
			switch (lua_type(  L, SECOND_ARGUMENT )  )
			{
				case LUA_TNUMBER:
					if ( lua_isinteger( L, SECOND_ARGUMENT ) )
					{
					    temp_int = lua_tointeger( L, SECOND_ARGUMENT );
						res = (eEEPROMRegTypeWrite( adr, (uint8_t *)&temp_int, INTEGER_DATA ) == EEPROM_OK )? SUCSESS : ERROR;
					}
					else
					{
						temp_float = lua_tonumber( L, SECOND_ARGUMENT);
						res = (eEEPROMRegTypeWrite( adr,(uint8_t *)&temp_float, NUMBER_DATA ) == EEPROM_OK )? SUCSESS : ERROR;
					}
					break;
				case LUA_TBOOLEAN:
					temp_bool = lua_toboolean( L, SECOND_ARGUMENT );
					res = (eEEPROMRegTypeWrite( adr,(uint8_t *)&temp_bool, BOOLEAN_DATA ) == EEPROM_OK )? SUCSESS : ERROR;
					break;
				default:
					break;
			}
			break;
		case ONE_ARGUMENT:
			RTC_ReadTime( RTC_FORMAT_BIN, &time_buffer );
		    RTC_ReadDate( RTC_FORMAT_BIN, &date_buffer);
			temp_time.Day 	 = date_buffer.date;
			temp_time.Month  = date_buffer.month;
			temp_time.Year 	 = date_buffer.year;
			temp_time.Hour 	 = time_buffer.hours;
			temp_time.Minute = time_buffer.minutes;
			temp_time.Second = time_buffer.seconds;
			res = (eEEPROMRegTypeWrite( adr,&temp_time, TIME_STAMP) == EEPROM_OK )? SUCSESS : ERROR;
			break;
		case SEVEN_ARGUMENT:
			temp_time.Day    = lua_tointeger( L, SECOND_ARGUMENT );
			temp_time.Month  = lua_tointeger( L, THIRD_ARGUMENT );
			temp_time.Year   = lua_tointeger( L, FOURTH_ARGUMENT );
			temp_time.Hour 	 = lua_tointeger( L, FIVE_ARGUMENT	);
			temp_time.Minute = lua_tointeger( L, SIX_ARGUMENT );
			temp_time.Second = lua_tointeger( L,  SEVEN_ARGUMENT  );
			res = (eEEPROMRegTypeWrite( adr,&temp_time, TIME_STAMP) == EEPROM_OK )? SUCSESS : ERROR;
			break;
		default: break;
	}
	lua_pushnumber(L, res );
	return ( ONE_RESULT );
}

 int iGetEEPROM( lua_State *L )
{
	uint8_t res = ERROR;
	float fdata;
	PDM_DATA_TIME data;
	if ( lua_gettop( L ) == ONE_ARGUMENT )
	{
		uint16_t adr = (uint16_t) lua_tointeger( L, FIRST_ARGUMENT );
		uint8_t d[REGISTER_SIZE];
		int idata;
		res = SUCSESS;
		switch ( eEEPROMReadRegister(adr, d) )
		{
				case INTEGER_DATA:
					    idata = * ((int *)d);
						lua_pushinteger( L, idata );
					break;
					case BOOLEAN_DATA:
						idata = * ((int *)d);
						lua_pushboolean( L, idata);
						break;
					case NUMBER_DATA:
						fdata = * ((float *)d) ;
						lua_pushnumber( L, fdata);
						break;
					case TIME_STAMP:
						vGetRegToTime( d,  &data);
						lua_pushnumber( L, data.Day);
						lua_pushnumber( L, data.Month);
						lua_pushnumber( L, data.Year);
						lua_pushnumber( L, data.Hour);
						lua_pushnumber( L, data.Minute);
						lua_pushnumber( L, data.Second);
						return (SEVEN_RESULT );
						break;
					default:
						res = ERROR;
						break;
		}
	}
	if (res == ERROR)  lua_pushnumber( L, NO_RESULT );
	lua_pushnumber( L, res);
	return ( TWO_RESULT );
}

/*
 *
 */
 int iSetRecord( lua_State *L )
 {
 	 int start_index = 0;
 	 uint8_t cd;
 	 uint16_t sd;
 	 uint8_t record[REGISTER_SIZE];
 	 uint32_t temp_int, temp_bool;
 	 float temp_float;
     if (eGetReocrdFieldsType( 0 ) == RECORD_TIME_STAMP)
     {
     	 vSetRecordData(0,0);
     	 start_index ++;
     }
 	 for (uint8_t i = 0; i < lua_gettop( L ) ; i++ )
 	 {
 			   switch ( eGetReocrdFieldsType( i + start_index ) )
 			   {
 			      case RECORD_ERROR:
 			      case RECORD_TIME_STAMP:
 			    	  break;
 		   	   	  case RECORD_BYTE:
 		   	      	  cd =lua_tointeger( L, i + 1 );
 		   	      	  vSetRecordData(i + start_index,&cd);
 		   	   		  break;
 		   	   	  case RECORD_SHORT:
                        sd = (uint16_t)lua_tointeger( L, i +1  );
 		   	     	  vSetRecordData(i + start_index,(uint8_t *)&sd);
 		   	   		  break;
 		   	   	  case RECORD_LUA:

 		   	   	    switch (lua_type(  L, i+1) )
 		   	   	    {
 		   	   	    		case LUA_TNUMBER:
 		   	   	    			if ( lua_isinteger( L, i+1 ) )
 		   	   	    			{
 		   	   	    				   temp_int = lua_tointeger( L, i +1 );
 		   	   	    				   vSetRegData(record,(uint8_t *)&temp_int,INTEGER_DATA  );
 		   	   	    		    }
 		   	   	    			else
 		   	   	    			{
 		   	   	    					temp_float = lua_tonumber( L, i+1);
 		   	   	    					vSetRegData(record,(uint8_t *)&temp_float,NUMBER_DATA  );
 		   	   	    			}
 		   	   	    			break;
 		   	   	    		case LUA_TBOOLEAN:
 		   	   	    		    temp_bool = lua_toboolean( L, i+1 );
 		   	   	    		    vSetRegData(record,(uint8_t *)&temp_bool,BOOLEAN_DATA );
 		   	   	    			break;
 		   	   	    		default:
 		   	   	    			break;
 		   	   	    }
 		   	    	vSetRecordData(i + start_index,record);
 		   	   		break;
 		   	   	  default: break;
 			   }
 	   }
 	   eEEPROMRecordADD();
 	  return ( NO_RESULT );
 }

 /*
  *
  */
 int iSetStorageFormat( lua_State *L )
 {

 	//RECORD_TIME_STAMP = 0x00,
 	//	RECORD_BYTE		  = 0x01,
 	//	RECORD_SHORT      = 0x02,
 	//	RECORD_LUA		  = 0x03,
 	//	RECORD_ERROR	  = 0xFF,

 uint8_t d[16]={RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP,
 			       RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP,
 			       RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP,RECORD_TIME_STAMP};
 	uint8_t arg_number = lua_gettop(L);

 	 if (arg_number!=0)
 	 {
 		 uint8_t reset_data = lua_tointeger( L , FIRST_ARGUMENT );
 		 uint8_t register_count = lua_tointeger( L , SECOND_ARGUMENT );
 		 uint8_t record_format_count = 0;
 		 for (uint8_t i = 0; i < arg_number -1; i++)
 		 {
 			 d[i] = lua_tointeger( L, i + 3 );
 			 record_format_count++;

 		 }
 		eCreateDataStorage(register_count , d, record_format_count, reset_data );

 	 }

 	return ( NO_RESULT );
 }



 /*
  * Функция установки из LUA времени и даты
  */
 int iSetTime( lua_State *L )
 {
	 RTC_TimeConfig_T time_buffer;
	 RTC_DateConfig_T date_buffer;
     if (lua_gettop(L) == SIX_ARGUMENT )
 	{
     	time_buffer.hours   = lua_tointeger( L , FIRST_ARGUMENT );
 	    time_buffer.minutes = lua_tointeger( L , SECOND_ARGUMENT  );
 		time_buffer.seconds = lua_tointeger( L , THIRD_ARGUMENT  );
 		date_buffer.date = lua_tointeger( L ,FOURTH_ARGUMENT  );
 		date_buffer.month = lua_tointeger( L ,FIVE_ARGUMENT);
 		date_buffer.year = lua_tointeger( L ,SIX_ARGUMENT );
 		RTC_ConfigTime(  RTC_FORMAT_BIN,  &time_buffer );
 		RTC_ConfigDate(  RTC_FORMAT_BIN,  &date_buffer );
 	}
 	return ( NO_RESULT );
 }
 /*
  * Функция чтения из LUA времени и даты
  */
int iGetTime( lua_State *L )
 {
	RTC_TimeConfig_T time_buffer;
	RTC_DateConfig_T date_buffer;
	RTC_ReadTime( RTC_FORMAT_BIN, &time_buffer );
	RTC_ReadDate( RTC_FORMAT_BIN, &date_buffer);
 	lua_pushinteger( L, time_buffer.hours );
 	lua_pushinteger( L, time_buffer.minutes );
 	lua_pushinteger( L,	time_buffer.seconds );
 	lua_pushinteger( L,	date_buffer.date  );
 	lua_pushinteger( L,	date_buffer.month );
 	lua_pushinteger( L,	date_buffer.year );
 	return (  SIX_ARGUMENT );
 }



/*
 * Setting discrete input configuration API function
 */
int iDinConfig(lua_State *L )
{
	int arg_number = lua_gettop(L);
	if (arg_number >= TWO_ARGUMENTS )  /*Check function argument count*/
	{
		eDinConfig( (uint8_t) lua_tointeger( L, FIRST_ARGUMENT ) -1U ,
				    (uint8_t) lua_tonumber( L, SECOND_ARGUMENT ) ,
					( arg_number == FOUR_ARGUMENTS ) ? ( uint32_t) lua_tointeger( L, FOURTH_ARGUMENT) :DEF_H_FRONT,
					( arg_number == FOUR_ARGUMENTS ) ? ( uint32_t) lua_tointeger( L, THIRD_ARGUMENT) : DEF_L_FRONT );
	}
	return ( NO_RESULT );
}

int iSetRPMConfig(lua_State *L )
{
	int arg_number = lua_gettop(L);
	if (arg_number >= TWO_ARGUMENTS )
	{
		float coof = ( arg_number >= THIRD_ARGUMENT) ? lua_tonumber( L, THIRD_ARGUMENT ) : 0;
		vSetRPMConfig((uint8_t) lua_tointeger( L, FIRST_ARGUMENT ) -1U, lua_tonumber( L, SECOND_ARGUMENT ), coof);
	}

	return ( NO_RESULT );
}

/*
 * Блок функций для работы с аналоговыми входами
 *
 */
/*
 *
 */
int isetAINCal (lua_State *L)
{
   uint16_t argument = lua_gettop(L);
   int res = 0;
   POINT_t points[2];
   uint8_t ucNumber =(uint8_t) lua_tointeger( L, FIRST_ARGUMENT);
   if ( ( argument >= FIVE_ARGUMENT ) && ( (argument -1)%2 == 0 ))
   {
       if ( eAinCalDataConfig(ucNumber -1 , (argument -1)/2 )== CAL_SUCCESS )
       {
           for (uint16_t i = 0; i < argument ; i = i + 2)
           {
               points[0].X =  lua_tonumber( L, i + 2 );
               points[0].Y =  lua_tonumber( L, i + 3);
               points[1].X =  lua_tonumber( L, i + 4);
               points[1].Y =  lua_tonumber( L, i + 5);
               eSetAinCalPoint(ucNumber-1,&points[0], i/2 );
           }
           res = 1;
       }
   }
   lua_pushboolean( L, res );
   return (ONE_RESULT);
}
/*
 *  Функции работы с выходами
 */
/*
 *
 */
int  iOutConfig( lua_State *L )
{
	if ( lua_gettop(L) >= SECOND_ARGUMENT)  //Проверяем, что при вызове нам передали нужное число аргументов
	{
		uint8_t out_number 		= ( uint8_t ) lua_tointeger( L, FIRST_ARGUMENT ) - 1U ; //Первым аргументом дожен передоваться номер канала
		float nominal_power     = ( float ) lua_tonumber(L, SECOND_ARGUMENT );										   // Номинальая мощность
		uint8_t reset_satate    = (lua_gettop(L) >= THIRD_ARGUMENT) ? ( uint8_t ) lua_tointeger(L, THIRD_ARGUMENT) : 1; // режим сброс. Можно ли перезапускать канал выключением
		uint16_t overload_time  = (lua_gettop(L) >= FOURTH_ARGUMENT) ? ( uint16_t ) lua_tointeger(L, FOURTH_ARGUMENT) : 0;
		float overlad_power     = (lua_gettop(L) >= FIVE_ARGUMENT) ? ( float ) lua_tonumber(L, FIVE_ARGUMENT ) : nominal_power; //Пусковой ток
		uint8_t filter_enabele  =  (lua_gettop(L) >= SIX_ARGUMENT) ? ( float ) lua_tonumber(L, SIX_ARGUMENT ) : 1;
		vHWOutOverloadConfig( out_number, nominal_power, overload_time  , overlad_power, reset_satate , filter_enabele);
	    vHWOutEnable(out_number);
	}
	return ( NO_RESULT );
}




/*
 *
 */
int  iOutResetConfig( lua_State *L )
{
	if (lua_gettop(L) >= THREE_ARGUMENTS )  //Проверяем, что при вызове нам передали нужное число аргументов
	{
		vHWOutResetConfig(( uint8_t ) lua_tointeger(L, FIRST_ARGUMENT ) -1U,
						  ( uint8_t ) lua_tointeger(L, SECOND_ARGUMENT ),
						  ( uint16_t) lua_tointeger(L, THIRD_ARGUMENT ));
	}
	return ( NO_RESULT );
}
/*
 *
 */
int  iOutSetPWM( lua_State *L )
{
	if (lua_gettop(L) == TWO_ARGUMENTS )  //Проверяем, что при вызове нам передали нужное число аргументов
	{
		vOutSetPWM( ( uint8_t ) lua_tointeger(L, FIRST_ARGUMENT ) -1U,
					( uint8_t) lua_tointeger( L, SECOND_ARGUMENT )
					);
	}
	return ( NO_RESULT );
}

/*
 *
 */
int isetPWMFreq(lua_State *L)
{
	if (lua_gettop(L) == TWO_ARGUMENTS)
	{
		uint8_t groupe = (uint8_t) lua_tointeger( L, FIRST_ARGUMENT);
		if  (groupe <= 5)
		{
			vPWMFreqSet( groupe , (uint32_t) lua_tointeger( L, SECOND_ARGUMENT));
		}
	}
	return ( NO_RESULT );
}
/*
 *   Блок функций для работы с CAN
 */
int iCanSendTable( lua_State *L )
{
	int arg_number = lua_gettop(L);
	uint8_t DATA[ CAN_FRAME_SIZE ];
	if (arg_number == CAN_SEND_TABLE_ARGUMENT_COUNT)  //Проверяем, что при вызове нам передали нужное число аргументов
	{
		if ( lua_istable(L, CAN_TABLE_POS ) )
		{
			uint8_t size 	= (uint8_t) lua_tointeger(L, FRAME_SIZE_POS);
			if  (size <= CAN_FRAME_SIZE )
			{
				for (uint8_t i = 0; i < size; i++)
				{
					lua_geti(L, CAN_TABLE_POS , i + 1);
					DATA[i]= lua_tointeger(L,-1);
					lua_pop(L,1);
				}
				vCanInsertTXData((uint32_t) lua_tointeger(L, FIRST_ARGUMENT ), &DATA[0], size);
			}
		}
	}
	return (  NO_RESULT );
}


/*
 *
 */
int iCanSendPDM( lua_State *L )
{
	uint8_t  DATA[CAN_FRAME_SIZE] = {0,0,0,0,0,0,0,0};
	uint8_t  size = lua_gettop(L);
	if (size >= TWO_ARGUMENTS)
	{
		size--;
		for (int i=0;i< size ;i++)
		{
			DATA[i]= (uint8_t) lua_tointeger(L,-( size-i)); /*Третьем агрументом должно передоватьс время плавного старта в милисекундах*/
		}
		vCanInsertTXData( (uint32_t)lua_tointeger(L, FIRST_ARGUMENT) , &DATA[0], 8);
	}
	return ( NO_RESULT );
}


/*
 *
 */
int iCanSendRequest( lua_State *L )
{
	int arg_number = lua_gettop(L);
	uint8_t DATA[CAN_FRAME_SIZE];
	if (arg_number >= SEND_REQUEST_ARGUMENT_COUNT)  //Проверяем, что при вызове нам передали нужное число аргументов
	{
		vSetWaitFilter( lua_tointeger( L, SECOND_ARGUMENT ) );//Второй агрумент - id ответного пакета
		uint8_t size  = arg_number -2;
		for (int i=0;i<size;i++)
		{
			DATA[i] = (uint8_t) lua_tointeger(L,-(arg_number-2-i)); //Третьем агрументом должно передоватьс время плавного старта в милисекундах
		}
		vCanInsertTXData( (uint32_t) lua_tointeger( L, FIRST_ARGUMENT ), &DATA[0], size);
	}
	return ( NO_RESULT );
}
/*
 * Устанавливаем новый фильтр, позволяющий принимать пакеты с нужным ID
 */
int iCanSetResiveFilter(lua_State *L )
{
  uint8_t ucResNumber = NO_RESULT;
  if (lua_gettop(L) == 1U )  /*Проверяем, что при вызове нам передали нужное число аргументов*/
  {
	  lua_pushnumber(L, eMailboxFilterSet( ( uint32_t ) lua_tointeger(L,-1))== BUFFER_FULL ? 1U : 0U );
	  ucResNumber = ONE_RESULT;
  }
  return ( ucResNumber );
}
/*
 * Проверка пакета с нужным id
 */
int iCanCheckData(lua_State *L )
{
	uint32_t uiRes = 0U;
	switch (lua_gettop(L))
	{
	  case 0U:
		  uiRes = vCheckAnswer();
		  break;
	  case 1U:
		  uiRes = vCanChekMessage( lua_tointeger(L, CAN_ID_POS) );
		  break;
	  default:
		  break;
	}
	lua_pushnumber(L, uiRes );
	return ( 1U );
}

int iCanGetMessage(lua_State *L )
{
	uint8_t n = 0;
	CAN_FRAME_TYPE  RXPacket;
	switch (lua_gettop(L))
	{
		case 0:
			n = vCanGetRequest(&RXPacket);
			break;
		case 1:
			luaL_checktype(L, 1, LUA_TNUMBER);
			RXPacket.ident = (uint32_t) lua_tointeger(L,1);
			n= vCanGetMessage(&RXPacket);
			break;
		default:
			break;
	}
	if (n!=0)
	{
		n = RXPacket.DLC;
		for (int i = 0; i <n;i++)
		{
			lua_pushnumber(L,RXPacket.data[i]);
		}
	}
	return ( n );
}

int iCanGetResivedData(lua_State *L )
{
	uint8_t n;
	CAN_FRAME_TYPE  RXPacket;
	switch (lua_gettop(L))
	{
	  case 1:
	    luaL_checktype(L, 1, LUA_TTABLE);
	    if  (vCanGetRequest(&RXPacket)!=0)
	    {
	      n = luaL_len(L, 1);
	    	for (int i = 1; i<(n+1);i++)
	    	{
	        lua_pushnumber(L,RXPacket.data[i-1]);
	      	lua_seti(L, 1, i);
	      }
	      lua_pushnumber(L,1U );
	    }
	    break;
	  case 2:
	    luaL_checktype(L, -1, LUA_TTABLE);
	    RXPacket.ident = (uint32_t) lua_tointeger(L,-2);
	    if ( vCanGetMessage(&RXPacket) == 1)
	    {
	      n = luaL_len(L, -1);
	    	for (int i = 1; i<(n+1);i++)
	    	{
	    	  lua_pushnumber(L,RXPacket.data[i-1]);
	    		lua_seti(L, -2, i);
	      }
	    	lua_pushnumber(L,1U );
	    }
	    else
	    {
	      lua_pushnumber(L,0U );
	    }
	    break;
	  default:
		  break;
	}
	return ( 1U );
}


/*
 *  Setting Can parameter API function
 */
int iCanSetConfig(lua_State *L)
{
	if (lua_gettop(L) == TWO_ARGUMENTS)
	{
		uint8_t ucCanNumber =(uint8_t) lua_tointeger( L, FIRST_ARGUMENT); //First argument it's channel number
		vCANBoudInit( (uint16_t)lua_tointeger( L, SECOND_ARGUMENT) );
	}
	return ( NO_RESULT );
}
/*
 *
 */


