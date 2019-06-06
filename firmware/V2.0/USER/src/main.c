#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "fifo.h"
#include "analysis_protocol.h"
#include "prjlib.h"
#include "rtc.h"
#include "moto.h"
#include "schedule.h" 
#include "wdg.h"
#include "key.h"
#include "ananlysis_data.h"
#include "prjlib.h"

#define HEAR_BEAT_TIME	45000

static void funControl(int argc, char* argv[]);
extern Moto motoDef;
extern mPin Pin_Array[PINMAX];
int time = 0;
extern _calendar_obj calendar;//时钟结构体 

int main(void)
{		
	CMD_ENT_DEF(MOTO, funControl);
	Cmd_AddEntrance(CMD_ENT(MOTO));
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  	uart1_init(115200);
	uart2_init(9600);
	Moto_Init();
	TIM3_Int_Init(HEAR_BEAT_TIME,7199);//10Khz的计数频率，计数到5000为500ms
	CLOSE_ELECTRIC_LOCK;

	// DBG_LOG("system start");
  if(RTC_Init())
		// DBG_LOG("RTC Init fail");
	IWDG_Init(6,1024);    //与分频数为64,重载值为625,溢出时间为1s	
	
  while(1) {
	IWDG_Feed();
	Gun_CommandReceive_Poll();
	Screen_CommandReceive_Poll();
	Start_Schedule();
	open_all_door();
	led_light();
	// test_fun();
	}
}

static void funControl(int argc, char* argv[]) {
	
	uint8_t i = 0;
  argv++;
  argc--;
  if(ARGV_EQUAL("ALL_OPEN")) 
	{  
	   for(i = 1;i < 57;i++) {
		   motoDef.open_moto(i);
		}
	}else if(ARGV_EQUAL("ALL_CLOSE")) 
	{  
	   for(i = 0;i < 57;i++)
		 {
		   motoDef.close_moto(i);
		 }
	}else if(ARGV_EQUAL("RUN")) 
	{
		  motoDef.open_moto(uatoi(argv[1]));
	}else if(ARGV_EQUAL("CLOSE")) 
	{  
		  motoDef.close_moto(uatoi(argv[1]));
	}else if(ARGV_EQUAL("motor_turn_off")) {
		PUSH_MOTOR(LEFT);
	}
}

void led_light(void) {
	RTC_Get();
	// delay_ms_whx(1000);
	// DBG_LOG("calendar.hour = %d",calendar.hour);
	// DBG_LOG("calendar.min = %d",calendar.min);
	if(calendar.hour <= 6) {
		// led turns off
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
		GPIO_ResetBits(GPIOD,GPIO_Pin_1);
	} else {
		// led turns on
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
		GPIO_SetBits(GPIOD,GPIO_Pin_1);
	}
}

void test_fun() {
	// // test borrow motor
	// motoDef.open_moto(2);
	// if(motoDef.read_moto(CHECK_TRACK)) {
	// 	DBG_LOG("signal feedback55");
	// 	motoDef.close_moto(2);
	// }

	// // test push motor
	// delay_ms(2000);
	// // motoDef.open_moto();
	// delay_ms(2000);

	// // test infrared
	// if (!(motoDef.read_moto(INFRARED_SENSOR_TEST))) {
	// 	// DBG_LOG("infrared!");
	// }
	// // test push motor
	// GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	// GPIO_SetBits(GPIOC,GPIO_Pin_10);
	// PUSH_MOTOR(RIGHT);  // out 
	// PUSH_MOTOR(LEFT);		//in

	// // test electric lock
	// 	IWDG_Feed();
	// 	OPEN_ELECTRIC_LOCK;
	// 	delay_ms_whx(5000);
	// 	IWDG_Feed();
	// 	CLOSE_ELECTRIC_LOCK;
	// 	delay_ms_whx(5000);
	// // crc16 test!
	// uint16_t crc_test;
	// uint8_t crc[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
	// crc_test = CRC_16(0xffff,crc+2,5);
	// // DBG_LOG("crc_test = 0x%04x",crc_test);

	// // led bug 
	// motoDef.open_moto(18);
	// if(motoDef.read_moto(CHECK_TRACK)) {
	// 	// DBG_LOG("test!");
	// 	// DBG_LOG("hello,world!");
	// }

	// // test unix time seting
	// uint8_t CmdRecBuf[18] = {0x7E,0x00,0x5C,0xE4,0xA6,0x42,0X07,0x53,0x43,0x41,\
	// 						 0x4E,0x20,0x4F,0x4E,0x00,0x63,0xBF,0x7E};
	// Get_Time(CmdRecBuf);
	// delay_ms_whx(1000);
	// // DBG_LOG("calendar.sec is %d",calendar.hour);

	// uint16_t data = 0xe267;
	// uint8_t data15 = 0, data16 = 0;
	// data15 = data;
	// data16 = data>>8;


	// // test input function
	// delay_ms_whx(1000);
	// static uint8_t report_data[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	// Report_State(0x80,(char*)report_data,sizeof(report_data));

	// // test new push motor
	// PUSH_MOTOR(LEFT);
	// PUSH_MOTOR_RIGHT;

	// // screen 
	// static uint8_t start_screen[6] = {0x04,0xE4,0x04,0x00,0xFF,0x14};
	// delay_ms(100);
	// Uart_Send_Data(GUN,start_screen,(sizeof(start_screen)-1));

	// success!!
	static uint8_t start_screen[6] = {0x04,0xE4,0x04,0x00,0xFF,0x14};
	delay_ms_whx(1000);
	Uart_Send_Data(GUN,start_screen,sizeof(start_screen)-1);
}
