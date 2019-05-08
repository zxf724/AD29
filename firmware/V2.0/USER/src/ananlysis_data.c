#include <time.h>
#include <stdio.h>
#include "ananlysis_data.h"
#include "moto.h"
#include "delay.h"
#include "string.h"
#include "analysis_protocol.h"
#include "usart.h"
#include "fifo.h"

extern Moto motoDef;
extern mError errorDef;
	
uint8_t g_start_cmd[7] = {0};
uint8_t g_bar_code[50] = {'\0'};

void Get_Time(uint8_t data[]) {
		uint32_t realtime = (data[2]<<24) | (data[3]<<16) | (data[4]<<8) | data[5];
    time_t time_seconds = (int)realtime;
    struct tm* now_time = localtime(&time_seconds);
		DBG_LOG("time seconds is %d",realtime);
    DBG_LOG("%d-%d-%d %d:%d:%d\n", now_time->tm_year + 1900, now_time->tm_mon + 1,
      now_time->tm_mday, now_time->tm_hour+8, now_time->tm_min,
      now_time->tm_sec);
			//time setting
		RTC_Set(now_time->tm_year+1900,now_time->tm_mon+1,now_time->tm_mday,
				now_time->tm_hour+8,now_time->tm_min,now_time->tm_sec);
}

void Get_Mote_Data(uint8_t* data)
{
	motoDef.num = *data;
	printf("rec mote %d",motoDef.num);
}

void Get_Lock_Data(uint8_t* data)
{
	motoDef.num = *data;
 	DBG_LOG("rec lock %d",motoDef.num);
}

void Get_Gun_Data(uint8_t* data)
{
  uint8_t len = 0;
	uint8_t i = 0;
  len =  *(data+4);
	for(i = 1;i <len+1;i++)
	{
	  g_start_cmd[i-1] = *(data+4+i);
	}
	// printf("rec Gun \r\n");
}

void Get_Cargo_Data(uint8_t* data)
{
  if(*(data+5) == 1)
	{
	  //收到出货信息
		errorDef.android_state = 1;
	}
}

void Get_Back_Data(uint8_t* data)
{
 // if(*(data+5) == 1)
	{
	  //收到扫码信息
		errorDef.android_state = 1;
	//  printf("rec bar \r\n");
	}
}   

void Get_Bar_Code(uint8_t* data)
{
	uint8_t i = 0;
	for(i = 0;i < *(data + 4);i++)
	 g_bar_code[i] = *(data+i);
	errorDef.bar_code_state = 1;
//	printf("andorid rec bar \r\n");
}

void Report_State(uint8_t cmd,uint8_t* data,uint8_t len)
{
	uint8_t i = 0;
  uint8_t report_data[100] = {0};
  report_data[0] = FHEADER;	
	report_data[1] = cmd;
	report_data[2] = 0;
	report_data[3] = 0;
	report_data[4] = 0;
	report_data[5] = 0;
	report_data[6] = len;
	for(i = 0;i<len;i++)
	{
	  report_data[7+i] = *(data+i);
	}
	report_data[7+i] = 0;
	report_data[8+i] = FEND;
	Uart_Send_Data(SCREEN,(char*)report_data,len+9);
}


