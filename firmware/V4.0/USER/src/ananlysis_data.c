#include "ananlysis_data.h"
#include <stdio.h>
#include <time.h>
#include "analysis_protocol.h"
#include "delay.h"
#include "fifo.h"
#include "moto.h"
#include "prjlib.h"
#include "rtc.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"

extern Moto motoDef;
extern mError errorDef;

uint8_t g_start_cmd[7] = {0};
uint8_t g_bar_code[25] = {'\0'};
uint8_t g_array_ML[8] = {0};

void Get_Time(uint8_t data[]) {
  uint32_t realtime =
      (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
  time_t time_seconds = (int)realtime; 
  struct tm* now_time = localtime(&time_seconds);
  // DBG_LOG("time seconds is %d", realtime);
  // DBG_LOG("%d-%d-%d %d:%d:%d\n", now_time->tm_year + 1900, now_time->tm_mon + 1,
  //         now_time->tm_mday, now_time->tm_hour + 9, now_time->tm_min,
  //         now_time->tm_sec);
  RTC_Set(now_time->tm_year + 1900, now_time->tm_mon + 1,
                       now_time->tm_mday, now_time->tm_hour + 9,
                       now_time->tm_min, now_time->tm_sec);
}

void Get_Mote_Data(uint8_t* data) {
  static uint8_t i = 0;
  // motoDef.num = *data;
 if(*data > 0) {
    g_array_ML[i] = *data;
    DBG_LOG("g_array_ML[%d] = %d",i,g_array_ML[i]);
    *data = 0;
    i++;
    if (i == 7) i = 0;
  }
}

void Get_Lock_Data(uint8_t* data) { motoDef.num = *data; }

void Get_Gun_Data(uint8_t* data) {
  static uint8_t i = 0;
  g_array_ML[i] = *data;
  *data = 0;
  i++;
  if (i == 7) i = 0;
    // motoDef.num = *data;
}

void Get_Cargo_Data(uint8_t* data) {
  if (*(data + 5) == 1) {
    //收到出货信息
    errorDef.android_state = 1;
  }
}

void Get_Back_Data(uint8_t* data) {
  // if(*(data+5) == 1)
  {
    //收到扫码信息
    errorDef.android_state = 1;
    //  printf("rec bar \r\n");
  }
}

void Get_Bar_Code(uint8_t* data) {
  uint8_t i = 0;
  for (i = 0; i < *(data + 4); i++) g_bar_code[i] = *(data + i);
  errorDef.bar_code_state = 1;
  //	printf("andorid rec bar \r\n");
}

void Report_State(uint8_t cmd, uint8_t* data, uint8_t len) {
  IWDG_Feed();
  uint8_t i = 0;
  uint8_t report_data[18] = {0};
  uint16_t crc_test;
  // get timestamp
  uint32_t timestamp = RTC_GetCounter();
  for (uint8_t i = 0; i <= 3; i++) {
    report_data[i + 2] = timestamp >> (i * 8);
  }
  report_data[0] = FHEADER;
  report_data[1] = cmd;
  report_data[6] = len;
  for (i = 0; i < len; i++) {
    report_data[7 + i] = *(data + i);
  }
  crc_test = CRC_16(0xffff, report_data + 1, 14);
  report_data[15] = crc_test;
  report_data[16] = crc_test >> 8;
  report_data[17] = FEND;

  Uart_Send_Data(SCREEN, (uint8_t*)report_data, 17);
}
